#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/custom.h>
#include <jwt.h>
#include <string.h>

#define RESULT_OR_ERROR(RESULT, VALUE, ERR)                 \
    ({ if (ERR == 0) {                                      \
        RESULT = alloc(2, 0);                               \
        Store_field(RESULT, 0, hash_variant("Ok"));         \
        Store_field(result, 1, VALUE);                      \
    } else {                                                \
        result = alloc(2, 0);                               \
        Store_field(RESULT, 0, hash_variant("Error"));      \
        Store_field(RESULT, 1, Val_int(ERR));               \
    }})

#define OK_OR_ERROR(RESULT, ERR)                            \
    ({ if (ERR == 0) {                                      \
        RESULT = hash_variant("Ok");                        \
    } else {                                                \
        result = alloc(2, 0);                               \
        Store_field(RESULT, 0, hash_variant("Error"));      \
        Store_field(RESULT, 1, Val_int(ERR));               \
    }})

#define Val_none Val_int(0)

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
    CAMLlocal2(result, ml_t);

    jwt_t *t;
    int err_code = jwt_new(&t);
    ml_t = wrap_ocaml_jwt(t);

    RESULT_OR_ERROR(result, ml_t, err_code);
    CAMLreturn(result);
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
    CAMLlocal2(result, ml_t);

    unsigned char* key_ptr = NULL;
    int key_len = 0;
    if (key != Val_none) {
        value field = Field(key, 0);
        key_ptr = (unsigned char*)String_val(field);
        key_len = string_length(field);
    }

    jwt_t *t;
    int err_code = jwt_decode(&t, String_val(token), key_ptr, key_len);
    ml_t = wrap_ocaml_jwt(t);

    RESULT_OR_ERROR(result, ml_t, err_code);
    CAMLreturn(result);
}

CAMLprim value ocaml_jwt_add_grant(value ml_t, value key, value val) {
    CAMLparam3(ml_t, key, val);
    CAMLlocal1(result);

    jwt_t *t = unwrap_ocaml_jwt(ml_t);
    int err_code = jwt_add_grant(t, String_val(key), String_val(val));

    OK_OR_ERROR(result, err_code);
    CAMLreturn(result);

}

CAMLprim value ocaml_jwt_add_grant_int(value ml_t, value key, value val) {
    CAMLparam3(ml_t, key, val);
    CAMLlocal1(result);

    jwt_t *t = unwrap_ocaml_jwt(ml_t);
    int err_code = jwt_add_grant_int(t, String_val(key), Long_val(val));

    OK_OR_ERROR(result, err_code);
    CAMLreturn(result);
}

CAMLprim value ocaml_jwt_add_grant_bool(value ml_t, value key, value val) {
    CAMLparam3(ml_t, key, val);
    CAMLlocal1(result);

    jwt_t *t = unwrap_ocaml_jwt(ml_t);
    int err_code = jwt_add_grant_bool(t, String_val(key), Bool_val(val));

    OK_OR_ERROR(result, err_code);
    CAMLreturn(result);
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

    result = Val_long(i);
    CAMLreturn(result);
}

CAMLprim value ocaml_jwt_get_grant_bool(value ml_t, value key) {
    CAMLparam2(ml_t, key);
    CAMLlocal1(result);

    jwt_t *t = unwrap_ocaml_jwt(ml_t);
    int i = jwt_get_grant_int(t, String_val(key));

    result = Val_bool(i);
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

    OK_OR_ERROR(result, err_code);
    CAMLreturn(result);
}

CAMLprim value ocaml_jwt_encode(value ml_t) {
    CAMLparam1(ml_t);
    CAMLlocal1(result);

    jwt_t *t = unwrap_ocaml_jwt(ml_t);
    char *s = jwt_encode_str(t);

    if (s != NULL) {
        result = alloc(2, 0);
        Store_field(result, 0, hash_variant("Ok"));
        Store_field(result, 1, caml_copy_string(s));
        free(s);
    } else {
        result = hash_variant("Error");
    }
    CAMLreturn(result);
}
