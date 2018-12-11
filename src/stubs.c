#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/custom.h>
#include <caml/fail.h>
#include <caml/callback.h>
#include <jwt.h>
#include <string.h>
#include <errno.h>

/*
 * Auxilliary macros and functions for C <-> OCaml conversion.
 */

#define Some_val(v) Field(v, 0)

#define Val_none Val_int(0)

value Val_some(value v) {
    CAMLparam1(v);
    CAMLlocal1(ml_some);
    ml_some = caml_alloc_small(1, 0);
    Store_field(ml_some, 0, v);
    CAMLreturn(ml_some);
}

enum result {OK = 0, ERROR = 1};

value Val_ok(value v) {
    CAMLparam1(v);
    CAMLlocal1(ml_result);
    ml_result = caml_alloc_small(1, OK);
    Store_field(ml_result, 0, v);
    CAMLreturn(ml_result);
}

value Val_jwt_error(int err_code) {
    CAMLparam0();
    CAMLlocal3(ml_hash, ml_variant, ml_result);
    ml_hash = caml_hash_variant("Jwt_error");
    ml_variant = caml_alloc_small(2, 0);
    Store_field(ml_variant, 0, ml_hash);
    Store_field(ml_variant, 1, Val_int(err_code));
    ml_result = caml_alloc_small(1, ERROR);
    Store_field(ml_result, 0, ml_variant);
    CAMLreturn(ml_result);
}

/*
 * Custom block wrapping of jwt_t so OCaml GC can manage it.
 */

jwt_t* unwrap_ocaml_jwt(value ml_jwt) {
    return *(jwt_t**)Data_custom_val(ml_jwt);
}

void finalize_ocaml_jwt(value ml_jwt) {
    CAMLparam1(ml_jwt);
    jwt_t *jwt = unwrap_ocaml_jwt(ml_jwt);
    jwt_free(jwt);
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

value wrap_ocaml_jwt(jwt_t *jwt) {
    value ml_jwt = caml_alloc_custom(&ocaml_jwt_custom_ops, sizeof(jwt), 0, 1);
    memcpy(Data_custom_val(ml_jwt), &jwt, sizeof(jwt));
    return ml_jwt;
}

/*
 * Stubs for OCaml external declarations.
 */

CAMLprim value ocaml_jwt_create(value ml_unit) {
    CAMLparam1(ml_unit);
    CAMLlocal2(ml_jwt, ml_result);

    jwt_t *jwt;
    int err_code = jwt_new(&jwt);

    if (err_code == 0) {
        ml_jwt = wrap_ocaml_jwt(jwt);
        ml_result = Val_ok(ml_jwt);
    }
    else {
        ml_result = Val_jwt_error(err_code);
    }

    CAMLreturn(ml_result);
}

CAMLprim value ocaml_jwt_encode(value ml_jwt) {
    CAMLparam1(ml_jwt);
    CAMLlocal2(ml_str, ml_result);

    jwt_t *jwt = unwrap_ocaml_jwt(ml_jwt);
    char *str = jwt_encode_str(jwt);

    if (str != NULL) {
        ml_str = caml_copy_string(str);
        ml_result = Val_ok(ml_str);
        free(str);
    }
    else {
        ml_result = Val_jwt_error(-1);
    }

    CAMLreturn(ml_result);
}

CAMLprim value ocaml_jwt_decode(value ml_key, value ml_token) {
    CAMLparam2(ml_key, ml_token);
    CAMLlocal2(ml_jwt, ml_result);

    unsigned char* key_ptr = NULL;
    int key_len = 0;
    if (ml_key != Val_none) {
        value field = Field(ml_key, 0);
        key_ptr = (unsigned char*)String_val(field);
        key_len = string_length(field);
    }

    jwt_t *jwt;
    int err_code = jwt_decode(&jwt, String_val(ml_token), key_ptr, key_len);
    if (err_code == 0) {
        ml_jwt = wrap_ocaml_jwt(jwt);
        ml_result = Val_ok(ml_jwt);
    }
    else {
        ml_result = Val_jwt_error(err_code);
    }

    CAMLreturn(ml_result);
}

CAMLprim value ocaml_jwt_dump(value ml_pretty, value ml_jwt) {
    CAMLparam2(ml_pretty, ml_jwt);
    CAMLlocal1(ml_dump);

    int pretty = ml_pretty == Val_none ? 0 : Bool_val(Field(ml_pretty, 0));
    jwt_t *jwt = unwrap_ocaml_jwt(ml_jwt);

    char *dump = jwt_dump_str(jwt, pretty);
    ml_dump = caml_copy_string(dump);
    free(dump);

    CAMLreturn(ml_dump);
}

CAMLprim value ocaml_jwt_add_grant(value ml_jwt, value ml_key, value ml_val) {
    CAMLparam3(ml_jwt, ml_key, ml_val);
    CAMLlocal1(ml_result);

    jwt_t *jwt = unwrap_ocaml_jwt(ml_jwt);
    int err_code = jwt_add_grant(jwt, String_val(ml_key), String_val(ml_val));

    if (err_code == 0) {
        ml_result = Val_ok(Val_unit);
    }
    else {
        ml_result = Val_jwt_error(err_code);
    }

    CAMLreturn(ml_result);
}

CAMLprim value ocaml_jwt_add_grant_int(value ml_jwt, value ml_key, value ml_val) {
    CAMLparam3(ml_jwt, ml_key, ml_val);
    CAMLlocal1(ml_result);

    jwt_t *jwt = unwrap_ocaml_jwt(ml_jwt);
    int err_code = jwt_add_grant_int(jwt, String_val(ml_key), Long_val(ml_val));

    if (err_code == 0) {
        ml_result = Val_ok(Val_unit);
    }
    else {
        ml_result = Val_jwt_error(err_code);
    }

    CAMLreturn(ml_result);
}

CAMLprim value ocaml_jwt_add_grant_bool(value ml_jwt, value ml_key, value ml_val) {
    CAMLparam3(ml_jwt, ml_key, ml_val);
    CAMLlocal1(ml_result);

    jwt_t *jwt = unwrap_ocaml_jwt(ml_jwt);
    int err_code = jwt_add_grant_bool(jwt, String_val(ml_key), Bool_val(ml_val));

    if (err_code == 0) {
        ml_result = Val_ok(Val_unit);
    }
    else {
        ml_result = Val_jwt_error(err_code);
    }

    CAMLreturn(ml_result);
}

CAMLprim value ocaml_jwt_add_grants_json(value ml_jwt, value ml_json) {
    CAMLparam2(ml_jwt, ml_json);
    CAMLlocal1(ml_result);

    jwt_t *jwt = unwrap_ocaml_jwt(ml_jwt);
    int err_code = jwt_add_grants_json(jwt, String_val(ml_json));

    if (err_code == 0) {
        ml_result = Val_ok(Val_unit);
    }
    else {
        ml_result = Val_jwt_error(err_code);
    }

    CAMLreturn(ml_result);
}

CAMLprim value ocaml_jwt_get_grant(value ml_jwt, value ml_key) {
    CAMLparam2(ml_jwt, ml_key);
    CAMLlocal2(ml_grant, ml_result);

    jwt_t *jwt = unwrap_ocaml_jwt(ml_jwt);
    const char *grant = jwt_get_grant(jwt, String_val(ml_key));

    if (grant == NULL) {
        ml_result = Val_none;
    } else {
        ml_grant = caml_copy_string(grant);
        ml_result = Val_some(ml_grant);
    }

    CAMLreturn(ml_result);
}

CAMLprim value ocaml_jwt_get_grant_int(value ml_jwt, value ml_key) {
    CAMLparam2(ml_jwt, ml_key);
    CAMLlocal1(ml_result);

    jwt_t *jwt = unwrap_ocaml_jwt(ml_jwt);
    long i = jwt_get_grant_int(jwt, String_val(ml_key));

    if (errno == ENOENT) {
        ml_result = Val_none;
    } else {
        ml_result = Val_some(Val_long(i));
    }

    CAMLreturn(ml_result);
}

CAMLprim value ocaml_jwt_get_grant_bool(value ml_jwt, value ml_key) {
    CAMLparam2(ml_jwt, ml_key);
    CAMLlocal1(ml_result);

    jwt_t *jwt = unwrap_ocaml_jwt(ml_jwt);
    int i = jwt_get_grant_bool(jwt, String_val(ml_key));

    if (errno == ENOENT) {
        ml_result = Val_none;
    } else {
        ml_result = Val_some(Val_bool(i));
    }

    CAMLreturn(ml_result);
}

CAMLprim value ocaml_jwt_get_grants_json(value ml_key, value ml_jwt) {
    CAMLparam2(ml_jwt, ml_key);
    CAMLlocal3(ml_field, ml_grant, ml_result);

    jwt_t *jwt = unwrap_ocaml_jwt(ml_jwt);
    char *grant;

    if (ml_key == Val_none) {
        grant = jwt_get_grants_json(jwt, NULL);
    } else {
        ml_field = Field(ml_key, 0);
        grant = jwt_get_grants_json(jwt, String_val(ml_field));
    }

    if (grant == NULL) {
        ml_result = Val_none;
    } else {
        ml_grant = caml_copy_string(grant);
        ml_result = Val_some(ml_grant);
        free(grant);
    }

    CAMLreturn(ml_result);
}

CAMLprim value ocaml_jwt_del_grant(value ml_jwt, value ml_key) {
    CAMLparam2(ml_jwt, ml_key);
    CAMLlocal1(ml_result);

    jwt_t *jwt = unwrap_ocaml_jwt(ml_jwt);
    int err_code = jwt_del_grants(jwt, String_val(ml_key));

    if (err_code == 0) {
        ml_result = Val_ok(Val_unit);
    }
    else {
        ml_result = Val_jwt_error(err_code);
    }

    CAMLreturn(ml_result);
}

CAMLprim value ocaml_jwt_del_grants(value ml_jwt) {
    CAMLparam1(ml_jwt);
    CAMLlocal1(ml_result);

    jwt_t *jwt = unwrap_ocaml_jwt(ml_jwt);
    int err_code = jwt_del_grants(jwt, NULL);

    if (err_code == 0) {
        ml_result = Val_ok(Val_unit);
    }
    else {
        ml_result = Val_jwt_error(err_code);
    }

    CAMLreturn(ml_result);
}

CAMLprim value ocaml_jwt_set_alg(value ml_key, value ml_jwt, value ml_alg_type) {
    CAMLparam3(ml_key, ml_jwt, ml_alg_type);
    CAMLlocal1(ml_result);

    unsigned char* key_ptr = NULL;
    int key_len = 0;
    if (ml_key != Val_none) {
        value field = Field(ml_key, 0);
        key_ptr = (unsigned char*)String_val(field);
        key_len = string_length(field);
    }

    jwt_t *jwt = unwrap_ocaml_jwt(ml_jwt);
    jwt_alg_t alg_type = Int_val(ml_alg_type);
    int err_code = jwt_set_alg(jwt, alg_type, key_ptr, key_len);

    if (err_code == 0) {
        ml_result = Val_ok(Val_unit);
    }
    else {
        ml_result = Val_jwt_error(err_code);
    }

    CAMLreturn(ml_result);
}

CAMLprim value ocaml_jwt_get_alg(value ml_jwt) {
    CAMLparam1(ml_jwt);
    CAMLlocal2(ml_alg_type, ml_result);

    jwt_t *jwt = unwrap_ocaml_jwt(ml_jwt);
    jwt_alg_t alg_type = jwt_get_alg(jwt);
    ml_alg_type = Val_int(alg_type);
    ml_result = Val_ok(ml_alg_type);

    CAMLreturn(ml_result);
}
