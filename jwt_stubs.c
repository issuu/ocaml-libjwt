#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <jwt.h>
#include <stdio.h>

#define RESULT_OR_ERROR(RESULT, VALUE, ERR)                 \
    ({ if (ERR == 0) {                                      \
        RESULT = alloc(2, 0);                               \
        Store_field(RESULT, 0, hash_variant("Ok"));         \
        Store_field(result, 1, (value)(VALUE));             \
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

CAMLprim value ocaml_jwt_create() {
    CAMLparam0();
    CAMLlocal1(result);
    jwt_t *t;
    int err_code = jwt_new(&t);
    RESULT_OR_ERROR(result, t, err_code);
    CAMLreturn(result);
}

CAMLprim value ocaml_jwt_decode(value key, value token) {
    CAMLparam2(key, token);
    CAMLlocal1(result);

    unsigned char* key_ptr = NULL;
    int key_len = 0;
    if (key != Val_none) {
        value field = Field(key, 0);
        key_ptr = (unsigned char*)String_val(field);
        key_len = string_length(field);
    }

    jwt_t *t;
    int err_code = jwt_decode(&t, String_val(token), key_ptr, key_len);
    RESULT_OR_ERROR(result, t, err_code);
    CAMLreturn(result);
}

CAMLprim value ocaml_jwt_dump(value pretty, value t) {
    CAMLparam2(pretty, t);
    CAMLlocal1(result);
    int pretty_ = pretty == Val_none ? 0 : Bool_val(Field(pretty, 0));
    char *s = jwt_dump_str((jwt_t*)t, pretty_);
    result = caml_copy_string(s);
    free(s);
    CAMLreturn(result);
}

CAMLprim value ocaml_jwt_add_grant(value t, value key, value val) {
    CAMLparam3(t, key, val);
    CAMLlocal1(result);
    int err_code = jwt_add_grant((jwt_t*)t, String_val(key), String_val(val));
    OK_OR_ERROR(result, err_code);
    CAMLreturn(result);

}

CAMLprim value ocaml_jwt_add_grant_int(value t, value key, value val) {
    CAMLparam3(t, key, val);
    CAMLlocal1(result);
    int err_code = jwt_add_grant_int((jwt_t*)t, String_val(key), Long_val(val));
    OK_OR_ERROR(result, err_code);
    CAMLreturn(result);
}

CAMLprim value ocaml_jwt_add_grant_bool(value t, value key, value val) {
    CAMLparam3(t, key, val);
    CAMLlocal1(result);
    int err_code = jwt_add_grant_bool((jwt_t*)t, String_val(key), Bool_val(val));
    OK_OR_ERROR(result, err_code);
    CAMLreturn(result);
}

CAMLprim value ocaml_jwt_get_grant(value t, value key) {
    CAMLparam2(t, key);
    CAMLlocal1(result);
    const char *s = jwt_get_grant((jwt_t*)t, String_val(key));
    if (s == NULL) {
        result = Val_none;
    } else {
        result = caml_alloc(1, 0);
        Store_field(result, 0, caml_copy_string(s));
    }
    CAMLreturn(result);
}

CAMLprim value ocaml_jwt_get_grant_int(value t, value key) {
    CAMLparam2(t, key);
    CAMLlocal1(result);
    long i = jwt_get_grant_int((jwt_t*)t, String_val(key));
    result = Val_long(i);
    CAMLreturn(result);
}

CAMLprim value ocaml_jwt_get_grant_bool(value t, value key) {
    CAMLparam2(t, key);
    CAMLlocal1(result);
    int i = jwt_get_grant_int((jwt_t*)t, String_val(key));
    result = Val_bool(i);
    CAMLreturn(result);
}

CAMLprim value ocaml_jwt_set_alg(value key, value t, value alg) {
    CAMLparam3(key, alg, t);
    CAMLlocal1(result);

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

    unsigned char* key_ptr = NULL;
    int key_len = 0;
    if (key != Val_none) {
        value field = Field(key, 0);
        key_ptr = (unsigned char*)String_val(field);
        key_len = string_length(field);
    }

    int err_code = jwt_set_alg((jwt_t*)t, alg_type, key_ptr, key_len);

    OK_OR_ERROR(result, err_code);
    CAMLreturn(result);
}

CAMLprim value ocaml_jwt_encode(value t) {
    CAMLparam1(t);
    CAMLlocal1(result);
    char *s = jwt_encode_str((jwt_t*)t);
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


