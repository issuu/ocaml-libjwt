type t

type error = [ `Jwt_error of string * int ]

val create : unit -> (t, [> error ]) result
val decode : ?key: string -> string -> (t, [> error ]) result

val get_grant : t -> string -> string option
val get_grant_int : t -> string -> int option
val get_grant_bool : t -> string -> bool option
val get_grants_json : ?key: string -> t -> string option

val add_grant : t -> string -> string -> (unit, [> error ]) result
val add_grant_int : t -> string -> int -> (unit, [> error ]) result
val add_grant_bool : t -> string -> bool -> (unit, [> error ]) result
val add_grants_json : t -> string -> (unit, [> error ]) result

val del_grant : t -> string -> (unit, [> error ]) result
val del_grants : t -> (unit, [> error ]) result

val dump : ?pretty: bool -> t -> string
val encode : t -> (string, [> error ]) result

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

val set_alg : ?key: string -> t -> algorithm -> (unit, [> error ]) result
val get_alg : t -> algorithm
