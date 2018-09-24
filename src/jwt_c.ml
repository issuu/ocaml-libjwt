type t

type error = [ `Jwt_error of string * int ]

exception Jwt_error of string * int

let () = Callback.register_exception "jwt exception" (Jwt_error ("<function name>", -1))

type algorithm =
  | NONE
  | HS256
  | HS384 
  | HS512 
  | RS256 
  | RS384 
  | RS512 
  | ES256 
  | ES384 
  | ES512 
  | TERM

external create_stub: unit -> t = "ocaml_jwt_create"
external decode_stub: ?key: string -> string -> t = "ocaml_jwt_decode"
external dump: ?pretty:bool -> t -> string = "ocaml_jwt_dump"

external add_grant_stub: t -> string -> string -> unit = "ocaml_jwt_add_grant"
external add_grant_int_stub: t -> string -> int -> unit = "ocaml_jwt_add_grant_int"
external add_grant_bool_stub: t -> string -> bool -> unit = "ocaml_jwt_add_grant_bool"
external add_grants_json_stub: t -> string -> unit = "ocaml_jwt_add_grants_json"

external get_grant : t -> string -> string option = "ocaml_jwt_get_grant"
external get_grant_int : t -> string -> int option = "ocaml_jwt_get_grant_int"
external get_grant_bool : t -> string -> bool option = "ocaml_jwt_get_grant_bool"
external get_grants_json : ?key: string -> t -> string option = "ocaml_jwt_get_grants_json"

external del_grant_stub : t -> string -> unit = "ocaml_jwt_del_grant"
external del_grants_stub : t -> unit = "ocaml_jwt_del_grants"

external encode_stub : t -> string = "ocaml_jwt_encode"

external set_alg_stub : ?key: string -> t -> algorithm -> unit = "ocaml_jwt_set_alg"
external get_alg : t -> algorithm = "ocaml_jwt_get_alg"

let wrap f arg =
  match f arg with
  | v -> Ok v
  | exception Jwt_error (func, err_code) -> Error (`Jwt_error (func, err_code))

let create () = wrap create_stub ()
let decode ?key token = wrap (decode_stub ?key) token

let add_grant jwt key value = wrap (add_grant_stub jwt key) value
let add_grant_int jwt key value = wrap (add_grant_int_stub jwt key) value
let add_grant_bool jwt key value = wrap (add_grant_bool_stub jwt key) value
let add_grants_json jwt json = wrap (add_grants_json_stub jwt) json

let del_grant jwt key = wrap (del_grant_stub jwt) key
let del_grants jwt = wrap del_grants_stub jwt

let encode jwt = wrap encode_stub jwt

let set_alg ?key jwt algorithm = wrap (set_alg_stub ?key jwt) algorithm
