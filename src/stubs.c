#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/custom.h>
#include <caml/fail.h>
#include <caml/callback.h>
#include <jwt.h>
#include <string.h>
#include <errno.h>

#define Val_none Val_int(0)

void ocaml_jwt_raise(const char* func_name, int err_code) {
    value args[] = { caml_copy_string(func_name), Val_int(err_code) };
    caml_raise_with_args(*caml_named_value("jwt exception"), 2, args);
}

// ========= custom block wrapping *jwt_t so OCaml GC can manage it ==========
jwt_t* unwrap_ocaml_jwt(value ml_t) {
    return *(jwt_t**)Data_custom_val(ml_t);
}

void finalize_ocaml_jwt(value ml_t) {
    CAMLparam1(ml_t);
    jwt_t *t = unwrap_ocaml_jwt(ml_t);
    // printf("FREEE!! %lx\n", (long)t);
    jwt_free(t);
    CAMLreturn0;
}

static struct custom_operations ocaml_jwt_custom_ops = {
    .identifier = "ocaml_jwt handling",
    .finalize = finalize_ocaml_jwt,
    .compare = custom_compare_default,
    .hash = custom_hash_default,
    .serialize = custom_serialize_default,
    .deserialize = custom_deserialize_default
};

value wrap_ocaml_jwt(jwt_t *t) {
    // printf("allocated: %lx\n", (long)t);
    value ml_t = caml_alloc_custom(&ocaml_jwt_custom_ops, sizeof(t), 0, 1);
    memcpy(Data_custom_val(ml_t), &t, sizeof(t));
    return ml_t;
}
// ===========================================================================

CAMLprim value ocaml_jwt_create() {
    CAMLparam0();
    CAMLlocal1(ml_t);

    jwt_t *t;
    int err_code = jwt_new(&t);

    if (err_code != 0) {
        ocaml_jwt_raise(__FUNCTION__, err_code);
    }
    ml_t = wrap_ocaml_jwt(t);
    CAMLreturn(ml_t);
}

CAMLprim value ocaml_jwt_dump(value pretty, value ml_t) {
    CAMLparam2(pretty, ml_t);
    CAMLlocal1(result);

    int pretty_ = pretty == Val_none ? 0 : Bool_val(Field(pretty, 0));
    jwt_t *t = unwrap_ocaml_jwt(ml_t);

    char *s = jwt_dump_str(t, pretty_);
    result = caml_copy_string(s);
    free(s);

    CAMLreturn(result);
}

CAMLprim value ocaml_jwt_decode(value key, value token) {
    CAMLparam2(key, token);
    CAMLlocal1(ml_t);

    unsigned char* key_ptr = NULL;
    int key_len = 0;
    if (key != Val_none) {
        value field = Field(key, 0);
        key_ptr = (unsigned char*)String_val(field);
        key_len = string_length(field);
    }

    jwt_t *t;
    int err_code = jwt_decode(&t, String_val(token), key_ptr, key_len);
    if (err_code != 0) {
        ocaml_jwt_raise(__FUNCTION__, err_code);
    }
    ml_t = wrap_ocaml_jwt(t);
    CAMLreturn(ml_t);
}

CAMLprim value ocaml_jwt_add_grant(value ml_t, value key, value val) {
    CAMLparam3(ml_t, key, val);

    jwt_t *t = unwrap_ocaml_jwt(ml_t);
    int err_code = jwt_add_grant(t, String_val(key), String_val(val));

    if (err_code != 0) {
        ocaml_jwt_raise(__FUNCTION__, err_code);
    }
    CAMLreturn(Val_unit);
}

CAMLprim value ocaml_jwt_add_grant_int(value ml_t, value key, value val) {
    CAMLparam3(ml_t, key, val);

    jwt_t *t = unwrap_ocaml_jwt(ml_t);
    int err_code = jwt_add_grant_int(t, String_val(key), Long_val(val));

    if (err_code != 0) {
        ocaml_jwt_raise(__FUNCTION__, err_code);
    }
    CAMLreturn(Val_unit);
}

CAMLprim value ocaml_jwt_add_grant_bool(value ml_t, value key, value val) {
    CAMLparam3(ml_t, key, val);

    jwt_t *t = unwrap_ocaml_jwt(ml_t);
    int err_code = jwt_add_grant_bool(t, String_val(key), Bool_val(val));

    if (err_code != 0) {
        ocaml_jwt_raise(__FUNCTION__, err_code);
    }
    CAMLreturn(Val_unit);
}

CAMLprim value ocaml_jwt_add_grants_json(value ml_t, value json) {
    CAMLparam2(ml_t, json);

    jwt_t *t = unwrap_ocaml_jwt(ml_t);
    int err_code = jwt_add_grants_json(t, String_val(json));

    if (err_code != 0) {
        ocaml_jwt_raise(__FUNCTION__, err_code);
    }
    CAMLreturn(Val_unit);
}

CAMLprim value ocaml_jwt_get_grant(value ml_t, value key) {
    CAMLparam2(ml_t, key);
    CAMLlocal1(result);

    jwt_t *t = unwrap_ocaml_jwt(ml_t);
    const char *s = jwt_get_grant(t, String_val(key));

    if (s == NULL) {
        result = Val_none;
    } else {
        result = caml_alloc(1, 0);
        Store_field(result, 0, caml_copy_string(s));
    }
    CAMLreturn(result);
}

CAMLprim value ocaml_jwt_get_grant_int(value ml_t, value key) {
    CAMLparam2(ml_t, key);
    CAMLlocal1(result);

    jwt_t *t = unwrap_ocaml_jwt(ml_t);
    long i = jwt_get_grant_int(t, String_val(key));

    if (i == 0 && errno == ENOENT) {
        result = Val_none;
    } else {
        result = caml_alloc(1, 0);
        Store_field(result, 0, Val_long(i));
    }

    CAMLreturn(result);
}

CAMLprim value ocaml_jwt_get_grant_bool(value ml_t, value key) {
    CAMLparam2(ml_t, key);
    CAMLlocal1(result);

    jwt_t *t = unwrap_ocaml_jwt(ml_t);
    int i = jwt_get_grant_bool(t, String_val(key));

    if (i == 0 && errno == ENOENT) {
        result = Val_none;
    } else {
        result = caml_alloc(1, 0);
        Store_field(result, 0, Val_bool(i));
    }

    CAMLreturn(result);
}

CAMLprim value ocaml_jwt_get_grants_json(value key, value ml_t) {
    CAMLparam2(ml_t, key);
    CAMLlocal1(result);

    jwt_t *t = unwrap_ocaml_jwt(ml_t);
    char *s;

    if (key == Val_none) {
        s = jwt_get_grants_json(t, NULL);
    } else {
        value field = Field(key, 0);
        s = jwt_get_grants_json(t, String_val(field));
    }

    if (s == NULL) {
        result = Val_none;
    } else {
        result = caml_alloc(1, 0);
        Store_field(result, 0, caml_copy_string(s));
        free(s);
    }
    CAMLreturn(result);
}

CAMLprim value ocaml_jwt_set_alg(value key, value ml_t, value alg) {
    CAMLparam3(key, ml_t, alg);
    CAMLlocal1(result);

    unsigned char* key_ptr = NULL;
    int key_len = 0;
    if (key != Val_none) {
        value field = Field(key, 0);
        key_ptr = (unsigned char*)String_val(field);
        key_len = string_length(field);
    }

    jwt_t *t = unwrap_ocaml_jwt(ml_t);

    jwt_alg_t alg_type;
    switch (Int_val(alg)) {
        case 0: alg_type = JWT_ALG_NONE; break;
        case 1: alg_type = JWT_ALG_HS256; break;
        case 2: alg_type = JWT_ALG_HS384; break;
        case 3: alg_type = JWT_ALG_HS512; break;
        case 4: alg_type = JWT_ALG_RS256; break;
        case 5: alg_type = JWT_ALG_RS384; break;
        case 6: alg_type = JWT_ALG_RS512; break;
        case 7: alg_type = JWT_ALG_ES256; break;
        case 8: alg_type = JWT_ALG_ES384; break;
        case 9: alg_type = JWT_ALG_ES512; break;
        case 10: alg_type = JWT_ALG_TERM; break;
    }

    int err_code = jwt_set_alg(t, alg_type, key_ptr, key_len);
    if (err_code != 0) {
        ocaml_jwt_raise(__FUNCTION__, err_code);
    }
    CAMLreturn(Val_unit);
}

CAMLprim value ocaml_jwt_get_alg(value ml_t) {
    CAMLparam1(ml_t);
    CAMLlocal1(result);

    jwt_t *t = unwrap_ocaml_jwt(ml_t);
    switch (jwt_get_alg(t)) {
        case JWT_ALG_NONE: result = Val_int(0); break;
        case JWT_ALG_HS256: result = Val_int(1); break;
        case JWT_ALG_HS384: result = Val_int(2); break;
        case JWT_ALG_HS512: result = Val_int(3); break;
        case JWT_ALG_RS256: result = Val_int(4); break;
        case JWT_ALG_RS384: result = Val_int(5); break;
        case JWT_ALG_RS512: result = Val_int(6); break;
        case JWT_ALG_ES256: result = Val_int(7); break;
        case JWT_ALG_ES384: result = Val_int(8); break;
        case JWT_ALG_ES512: result = Val_int(9); break;
        case JWT_ALG_TERM: result = Val_int(10); break;
    }
    CAMLreturn(result);
}


CAMLprim value ocaml_jwt_del_grant(value ml_t, value key) {
    CAMLparam2(ml_t, key);

    jwt_t *t = unwrap_ocaml_jwt(ml_t);
    int err_code = jwt_del_grants(t, String_val(key));

    if (err_code != 0) {
        ocaml_jwt_raise(__FUNCTION__, err_code);
    }
    CAMLreturn(Val_unit);
}

CAMLprim value ocaml_jwt_del_grants(value ml_t) {
    CAMLparam1(ml_t);

    jwt_t *t = unwrap_ocaml_jwt(ml_t);
    int err_code = jwt_del_grants(t, NULL);

    if (err_code != 0) {
        ocaml_jwt_raise(__FUNCTION__, err_code);
    }
    CAMLreturn(Val_unit);
}

CAMLprim value ocaml_jwt_encode(value ml_t) {
    CAMLparam1(ml_t);
    CAMLlocal1(result);

    jwt_t *t = unwrap_ocaml_jwt(ml_t);
    char *s = jwt_encode_str(t);

    if (s == NULL) {
        ocaml_jwt_raise(__FUNCTION__, -1);
    } else {
        result = caml_copy_string(s); 
        free(s);
    }
    CAMLreturn(result);
}
