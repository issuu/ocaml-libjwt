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

let get_set_grant_string () =
  let jwt = Jwt.create () in
  Alcotest.(check (option string)) "non-existing" None (Jwt.get_grant jwt "non-existing");
  Jwt.add_grant jwt "abc" "efg";
  Alcotest.(check (option string)) "existing" (Some "efg") (Jwt.get_grant jwt "abc");
  Gc.full_major ()

let get_set_grant_int () =
  let jwt = Jwt.create () in
  Alcotest.(check (option int)) "non-existing" None (Jwt.get_grant_int jwt "non-existing");
  Jwt.add_grant_int jwt "n" 42;
  Alcotest.(check (option int)) "existing" (Some 42) (Jwt.get_grant_int jwt "n");
  Jwt.add_grant_int jwt "zero" 0;
  Alcotest.(check (option int)) "zero" (Some 0) (Jwt.get_grant_int jwt "zero");
  Gc.full_major ()

let get_set_grant_bool () =
  let jwt = Jwt.create () in
  Alcotest.(check (option bool)) "non-existing" None (Jwt.get_grant_bool jwt "non-existing");
  Jwt.add_grant_bool jwt "true" true;
  Alcotest.(check (option bool)) "true" (Some true) (Jwt.get_grant_bool jwt "true");
  Jwt.add_grant_bool jwt "false" false;
  Alcotest.(check (option bool)) "false" (Some false) (Jwt.get_grant_bool jwt "false");
  Gc.full_major ()

let get_json () =
  let jwt = Jwt.create () in
  Jwt.add_grant jwt "abc" "efg";
  Jwt.add_grant_int jwt "n" 42;
  Jwt.add_grant_bool jwt "b" true;
  Alcotest.(check (option string)) "json" (Some {json|{"abc":"efg","b":true,"n":42}|json}) (Jwt.get_grants_json jwt);
  Alcotest.(check (option string)) "with key" (Some {json|"efg"|json}) (Jwt.get_grants_json ~key:"abc" jwt);
  Alcotest.(check (option string)) "non-existing" None (Jwt.get_grants_json ~key:"non-existing" jwt);
  Gc.full_major ()

let set_json () =
  let jwt = Jwt.create () in
  Jwt.add_grants_json jwt {json|{"abc":"efg","b":true,"n":42}|json};
  Alcotest.(check (option string)) "json" (Some {json|{"abc":"efg","b":true,"n":42}|json}) (Jwt.get_grants_json jwt);
  Alcotest.(check (option string)) "get grant" (Some "efg") (Jwt.get_grant jwt "abc");
  Gc.full_major ()

let delete_grant () =
  let jwt = Jwt.create () in
  Jwt.add_grants_json jwt {json|{"abc":"efg","b":true,"n":42}|json};
  Jwt.del_grant jwt "abc";
  Alcotest.(check (option string)) "json" (Some {json|{"b":true,"n":42}|json}) (Jwt.get_grants_json jwt);
  Gc.full_major ()

let delete_all_grants () =
  let jwt = Jwt.create () in
  Jwt.add_grants_json jwt {json|{"abc":"efg","b":true,"n":42}|json};
  Jwt.del_grants jwt;
  Alcotest.(check (option string)) "json" (Some "{}") (Jwt.get_grants_json jwt);
  Gc.full_major ()

let decode_encode () =
  let jwt = Jwt.create () in
  let now = Unix.time () |> int_of_float in
  Jwt.set_alg jwt RS256 ~key:private_key;
  Jwt.add_grant jwt "iss" "https://www.googleapis.com/robot/v1/metadata/x509/recommendation-tracking%40recommendation-tracking.iam.gserviceaccount.com";
  Jwt.add_grant jwt "scope" "https://www.googleapis.com/auth/prediction";
  Jwt.add_grant jwt "aud" "https://www.googleapis.com/oauth2/v4/token";
  Jwt.add_grant_int jwt "exp" (now + 3600);
  Jwt.add_grant_int jwt "iat" now;
  let jwt_encoded = Jwt.encode jwt in
  let jwt_decoded = Jwt.decode jwt_encoded ~key:public_key in
  Alcotest.(check (option string)) "json" (Jwt.get_grants_json jwt) (Jwt.get_grants_json jwt_decoded);
  Gc.full_major ()

let () =
  Alcotest.run "Jwt_C" [
    "tests", [
      "get/set grant (string)", `Quick, get_set_grant_string;
      "get/set grant (int)", `Quick, get_set_grant_int;
      "get/set grant (bool)", `Quick, get_set_grant_bool;
      "get json", `Quick, get_json;
      "set json", `Quick, set_json;
      "delete grant", `Quick, delete_grant;
      "delete all grants", `Quick, delete_all_grants;
      "decode / encode", `Quick, decode_encode;
    ]
  ]
