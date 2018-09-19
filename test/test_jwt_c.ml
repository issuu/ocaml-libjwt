module Jwt = Jwt_c

(* the key is just something I generated with `openssl genrsa -out private.pem 2048` *)
let private_key = "-----BEGIN RSA PRIVATE KEY-----
MIIEpQIBAAKCAQEA2EobPLULbXZNI9GBoXvX2X4Yplh4KIhDBWd33ll4id+Wsw6/
vIvOKy2rB74zcTgmRi6ZVNArgX5jdC6+nJHer33DQl/CNls24n7H5b/gC9O253ro
JjYQU9FiwMH4nsneAJimXizNc00E5pqhvMbCu1/ZoKb6hLBrcLF1KA3n38GTcmaq
r9yI5TM65B2pKelNAbBaIB2oBWAKjPfvDLkOhyNrgAKV677LD3fsU+lC+ea9ylt/
KPoicOAQFZer1kqYcI+IeouG2ePJcPEH3xpH0mFtaTzmJd5Hzr84kmM2NZRN+oOl
OYOKFluSTWQ1aojPQBmhceQFnfLAKoOW4qS6VQIDAQABAoIBAQCh8NJ14KP+wD2/
nMSvUNdrS6NPIRxOVL/BkxfHzjuXF2ZxCJD8rbyezaGpuXowwja+A3PgccCxQx1Z
xwoGlp0hzkrdLm9uXVs5uG0ZE1G/6TOgG4En4wDUkQichF8PHNvwnFqsVmU5eCg8
NPj2K+dvfbOnOn6FzMWU6flrFQZQm/HCB0igqyd85kfb9/Cj9iT2Jf9hRA1fhhBE
q+VyHISSD9QOO3E3S8mrHMc6otZdIwT8MWMh0YaJsYSV4/cPe9nVODV9PLPkVgz4
COZcGYfBDElvWo5IPZRcD47rdJx9j2eqnE4dhAOsFSH2qgsQKU566ZAX14DUUCZ6
Pk+6uhRdAoGBAPCyoK94iZ5A9+wc9MdpgyRKlq7RA2zt0GR+47+Liv614+zJAnc1
qIdEqIcDPdMILQbWBIMwtwxYiSQ1ILuL6IfMIsXz3Jpd+BGnV3EtJFg0/TshPu/U
hQsgOjShajemzP8oT3AGcqWmCa6xH27wbPgT9gIa7XkS64kRqBOLZCATAoGBAOYK
O3J0RDgUTWWWBT2INNIn9S1/B7TmHbER3ESCP+f0k5GRvK5LLAqWNrrdGWN5s/xQ
tRWMYOfYDbkyAsolS3vKpo/m3upkTD/G8J0faA/P30wvq3XOEKstbqeiYlAofeVz
TI1sHAutJx0OtOdQB9L/YDbb7JCqY3dBZqeG2Bj3AoGAWyF2fxknGZjFhUrtTnbf
ZSUsaeHO5zYfGX2FYydFJ+zb7/GnElVpilVvbTbH+Jd23Mi5CqauF2mJ+wB2dSui
jY+3drU+x99eJejyzXHm+dKOMg4DUzBmcvDvuK+IlKt9n/m2Idb/H0J/FfoPyaQT
zdVY5jElyhpkvQ/HUCcNcKsCgYEAqUfQVTpP1UDDb8UFGDG6RQhYeOFo52sLYHk3
MUb0BhpJ2a54PX5d63B81+fKPhSaKUuu5iuGSDYKa0TtHppxQKhxB6YqViv2nFwm
RkmNR01+Ec9mimtYgs8NBdkOJdSWSJofNFbhEIqcJNrkru+Kwm9g+x29qPtp9KEx
DIjDTyMCgYEAkdv4S187YxFAXGgcHAKVBP1awardfyspc1sUBa/ABGQl6JRLlI/W
xNCvRhfRxa5mCCLYhCRXIm74JEQC7craX+K0ln83YZ7870HuwNEanuqkIRQhMP1g
qA2xRVxCNmkZ/Ju6i1dXwmVomoECSAYRLqHbRHScSfqqFr2rAec9BQg=
-----END RSA PRIVATE KEY-----"

let public_key = "-----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA2EobPLULbXZNI9GBoXvX
2X4Yplh4KIhDBWd33ll4id+Wsw6/vIvOKy2rB74zcTgmRi6ZVNArgX5jdC6+nJHe
r33DQl/CNls24n7H5b/gC9O253roJjYQU9FiwMH4nsneAJimXizNc00E5pqhvMbC
u1/ZoKb6hLBrcLF1KA3n38GTcmaqr9yI5TM65B2pKelNAbBaIB2oBWAKjPfvDLkO
hyNrgAKV677LD3fsU+lC+ea9ylt/KPoicOAQFZer1kqYcI+IeouG2ePJcPEH3xpH
0mFtaTzmJd5Hzr84kmM2NZRN+oOlOYOKFluSTWQ1aojPQBmhceQFnfLAKoOW4qS6
VQIDAQAB
-----END PUBLIC KEY-----"

let () =
  let jwt = Jwt.create () in
  let now = Unix.time () |> int_of_float in
  Printf.printf "NOW: %d\n" now;
  Jwt.set_alg jwt RS256 ~key:private_key;
  Jwt.add_grant jwt "iss" "https://www.googleapis.com/robot/v1/metadata/x509/recommendation-tracking%40recommendation-tracking.iam.gserviceaccount.com";
  Jwt.add_grant jwt "scope" "https://www.googleapis.com/auth/prediction";
  Jwt.add_grant jwt "aud" "https://www.googleapis.com/oauth2/v4/token";
  Jwt.add_grant_int jwt "exp" (now + 3600);
  Jwt.add_grant_int jwt "iat" now;
  Printf.printf "%s\n" @@ Jwt.dump ~pretty:true jwt;
  Printf.printf "%s\n" (Jwt.get_grant jwt "iss" |> function Some s -> s | None -> "<None>");
  Printf.printf "%d\n" @@ Jwt.get_grant_int jwt "iat";
  let jwt_encoded = Jwt.encode jwt in
  Printf.printf "\n\nEncoded:\n%s\n" jwt_encoded;
  Printf.printf "DECODED JWT: %s\n" (Jwt.dump ~pretty:true @@ Jwt.decode jwt_encoded ~key:public_key);

  Printf.printf "%s\n" ((Jwt.get_grants_json ~key:"scope" jwt) |> function Some s -> s | None -> "<None>");
  Printf.printf "%s\n" ((Jwt.get_grants_json jwt) |> function Some s -> s | None -> "<None>");

  Printf.printf "%b\n" @@ ((Jwt.get_alg jwt) = RS256);

  Gc.full_major ();
  Printf.printf "DONE!\n"

let create_google_auth_payload ~key ~client_id =
  let now = Unix.time () |> int_of_float in
  let jwt = Jwt.create () in
  Jwt.set_alg jwt RS256 ~key;
  Jwt.add_grant jwt "aud" "https://www.googleapis.com/oauth2/v4/token";
  Jwt.add_grant jwt "iss" client_id;
  Jwt.add_grant jwt "scope" "https://www.googleapis.com/auth/cloud-platform";
  Jwt.add_grant_int jwt "exp" (now + 36);
  Jwt.add_grant_int jwt "iat" now;
  Jwt.encode jwt

(*
let authenticate_with_google ~private_key ~client_email =
  let assertion = create_google_auth_payload ~key:private_key ~client_id:client_email in
  let headers = Header.init_with "content-type" "application/x-www-form-urlencoded" in
  let body = Uri.encoded_of_query [
    ("assertion", [assertion]);
    ("grant_type", ["urn:ietf:params:oauth:grant-type:jwt-bearer"])
  ] |> Cohttp_lwt.Body.of_string in
  Client.post ~headers ~body (Uri.of_string "https://www.googleapis.com/oauth2/v4/token") >>= fun (rsp, rsp_body) ->
  let code = rsp |> Response.status |> Code.code_of_status in
  Printf.printf "Response code: %d\n" code;
  Printf.printf "Response Headers: %s\n" (rsp |> Response.headers |> Header.to_string);
  rsp_body |> Cohttp_lwt.Body.to_string >|= fun body ->
  Printf.printf "Body of length: %d\n" (String.length body);
  Printf.printf "Body: %s\n" body

let () =
  Lwt_main.run (authenticate_with_google
    ~private_key:"-----BEGIN PRIVATE KEY-----\n[INSERT REAL KEY HERE]\n-----END PRIVATE KEY-----"
    ~client_email:"[INSERT REAL CLIENT EMAIL HERE")
  |> fun hest ->
      ignore hest
*)
