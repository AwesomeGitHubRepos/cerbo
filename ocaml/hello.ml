print_string "Hello world!\n";;


let revs = [30814.0 ;	33974.0 ; 	39454.0 ;	42641.0 ;
	47298.0 ;	54327.0 ;	56910.0 ;	60931.0 ;
	64539.0 ;	64826.0 ;	63873.7 ;	65554.7 ];;

let pfloats lst =  List.iter (Printf.printf "%f\n" ) lst;;

(* let red accum a b rest =  b lst = b/.a :: [b *)

let rec ratios lst  = match lst with
  (* a::b::[] -> [b/. a] *)
  (* a::(b::rest) ->  b /.a::(ratios (b:: rest))::[]*)
  (* a::b::rest -> 1::a::2::b::3::rest *)
  [] -> []
  | a::[] -> []
  (* | a::b::[] -> [666.0] *)
  | a::b::rest -> (b/.a)::(ratios (b::rest))
  (* | _ -> [777.0] *)
  ;;

#trace ratios ;;

let r = ratios revs ;;

pfloats r;;

let r1 = ratios [] ;;

(*
let () = List.iter (Printf.printf "%f\n" ) r
*)
(* [3.0 ; 4.0 ; 5.0] *)

