type t

type error = [ `Jwt_error of int ]

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

external create : unit -> (t, [> error ]) result = "ocaml_jwt_create"
external encode : t -> (string, [> error ]) result = "ocaml_jwt_encode"
external decode : ?key:string -> string -> (t, [> error ]) result = "ocaml_jwt_decode"
external dump: ?pretty:bool -> t -> string = "ocaml_jwt_dump"

external add_grant : t -> string -> string -> (unit, [> error ]) result = "ocaml_jwt_add_grant"
external add_grant_int : t -> string -> int -> (unit, [> error ]) result = "ocaml_jwt_add_grant_int"
external add_grant_bool : t -> string -> bool -> (unit, [> error ]) result = "ocaml_jwt_add_grant_bool"
external add_grants_json : t -> string -> (unit, [> error ]) result = "ocaml_jwt_add_grants_json"

external get_grant : t -> string -> string option = "ocaml_jwt_get_grant"
external get_grant_int : t -> string -> int option = "ocaml_jwt_get_grant_int"
external get_grant_bool : t -> string -> bool option = "ocaml_jwt_get_grant_bool"
external get_grants_json : ?key:string -> t -> string option = "ocaml_jwt_get_grants_json"

external del_grant : t -> string -> (unit, [> error ]) result = "ocaml_jwt_del_grant"
external del_grants : t -> (unit, [> error ]) result = "ocaml_jwt_del_grants"

external set_alg : ?key:string -> t -> algorithm -> (unit, [> error ]) result = "ocaml_jwt_set_alg"
external get_alg : t -> algorithm = "ocaml_jwt_get_alg"
