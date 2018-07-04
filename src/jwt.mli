
type t

exception Jwt_error of string * int

val create : unit -> t
val decode : ?key: string -> string -> t

val get_grant : t -> string -> string option
val get_grant_int : t -> string -> int
val get_grant_bool : t -> string -> bool
(* val get_grants_json : t -> string -> string option *)
val add_grant : t -> string -> string -> unit
val add_grant_int : t -> string -> int -> unit
val add_grant_bool : t -> string -> bool -> unit
(* val add_grants_json : t -> string -> unit *)

(*
val del_grants : t -> [ `Ok | `Error of int ]
val del_grant : t -> string -> [ `Ok | `Error of int ]
*)

val dump : ?pretty: bool -> t -> string
val encode : t -> string

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

val set_alg : ?key: string -> t -> jwt_alg -> unit
(* val get_alg : t -> jwt_alg *)
