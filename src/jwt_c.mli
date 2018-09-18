type t

exception Jwt_error of string * int

val create : unit -> t
val decode : ?key: string -> string -> t

val get_grant : t -> string -> string option
val get_grant_int : t -> string -> int
val get_grant_bool : t -> string -> bool
val get_grants_json : t -> string -> string option
val add_grant : t -> string -> string -> unit
val add_grant_int : t -> string -> int -> unit
val add_grant_bool : t -> string -> bool -> unit
val add_grants_json : t -> string -> unit

val del_grant : t -> string -> unit
val del_grants : t -> unit

val dump : ?pretty: bool -> t -> string
val encode : t -> string

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

val set_alg : ?key: string -> t -> algorithm -> unit
val get_alg : t -> algorithm
