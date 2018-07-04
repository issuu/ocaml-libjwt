type t

type jwt_alg =
  | JWT_ALG_NONE
  | JWT_ALG_HS256
  | JWT_ALG_HS384 
  | JWT_ALG_HS512 
  | JWT_ALG_RS256 
  | JWT_ALG_RS384 
  | JWT_ALG_RS512 
  | JWT_ALG_ES256 
  | JWT_ALG_ES384 
  | JWT_ALG_ES512 
  | JWT_ALG_TERM

external create: unit -> [`Ok of t | `Error of int] = "ocaml_jwt_create"
external decode: ?key: string -> string -> [ `Ok of t | `Error of int ] = "ocaml_jwt_decode"
external dump: ?pretty:bool -> t -> string = "ocaml_jwt_dump"
external add_grant: t -> string -> string -> [`Ok | `Error of int] = "ocaml_jwt_add_grant"
external add_grant_int: t -> string -> int -> [`Ok | `Error of int] = "ocaml_jwt_add_grant_int"
external add_grant_bool: t -> string -> bool -> [`Ok | `Error of int] = "ocaml_jwt_add_grant_bool"
external get_grant : t -> string -> string option = "ocaml_jwt_get_grant"
external get_grant_int : t -> string -> int = "ocaml_jwt_get_grant_int"
external get_grant_bool : t -> string -> bool = "ocaml_jwt_get_grant_bool"
external encode : t -> [`Ok of string | `Error ] = "ocaml_jwt_encode"
external set_alg : ?key: string -> t -> jwt_alg -> [`Ok | `Error of int] = "ocaml_jwt_set_alg"

