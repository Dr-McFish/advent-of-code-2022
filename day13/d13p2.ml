open Printf

let input_fl_name = "in.txt"

let read_file filename = 
  let lines = ref [] in
  let chan = open_in filename in
  try
    while true; do
      lines := ((input_line chan)) :: !lines
    done; !lines
  with End_of_file ->
    close_in chan;
    List.rev !lines ;;


type int_or_list = N of int | L of int_or_list list

let get_closing_bracket = 
  let rec get_closing_bracket_rec (bracket_accumulator :int) (charcount_accumulator :int) (str :string) : int =
    if((String.length str) = 0) then
      charcount_accumulator
    else
      let head = String.get str 0 in
      let tail = String.sub str 1 ((String.length str) - 1) in
      if head = '[' then
        get_closing_bracket_rec (bracket_accumulator +1) (charcount_accumulator +1) tail
      else if head = ']' then
        if bracket_accumulator = 1 then
          charcount_accumulator
        else
          get_closing_bracket_rec (bracket_accumulator -1) (charcount_accumulator +1) tail
      else
        get_closing_bracket_rec (bracket_accumulator) (charcount_accumulator +1) tail
  in
  get_closing_bracket_rec 0 0

let is_digit (c: char) : bool = 
  (Char.code c >= Char.code '0') && (Char.code c <= Char.code '9')
let get_num_end (str: string) :int =
  let rec get_num_end_rec (charcount_accumulator :int) (str: string) =
    if((String.length str) = 0) then
      charcount_accumulator
    else
      let head = String.get str 0 in
      let tail = String.sub str 1 ((String.length str) - 1) in
      if (is_digit head) then
        get_num_end_rec (charcount_accumulator + 1) tail
      else
        charcount_accumulator
  in get_num_end_rec 0 str


                                           (* head   tail *)
let extract_substring_list (str: string) : (string * string) =
  let first_char = String.get str 0 in
  assert (first_char = '[') ;
  let closing_bracket = get_closing_bracket str in
  let head = String.sub str 1 (closing_bracket - 1) in
  let tail = String.sub str (closing_bracket + 1) ((String.length str) - closing_bracket - 1) in
  (head, tail)

let extract_substring_number (str: string) : (string * string) =
  let first_char = String.get str 0 in
  assert (is_digit first_char);
  let num_end = get_num_end str in
  let head = if num_end = 0 then 
    "" else String.sub str 0 num_end in
  let tail = String.sub str num_end ((String.length str) - num_end) in
  (head, tail)

let rec int_or_list_list_of_string (line : string) : int_or_list list = 
  if (String.length line = 0) then
    []
  else
    let first_char = String.get line 0 in
    if first_char = '[' then
      let (head, tail) = extract_substring_list line in
      if 0 < String.length tail then
        let () = try assert ((1 < String.length tail) && (',' = String.get tail 0)) 
          with e-> printf "len %d, %c\n" (String.length tail) (String.get tail 0) in
        let tail_without_comma = String.sub tail 1 (String.length tail -1) in
        L(int_or_list_list_of_string head) :: (int_or_list_list_of_string tail_without_comma) (* extract sublist, then recursive call *)
      else
        L(int_or_list_list_of_string head) :: []
    else
      let () = try assert (is_digit first_char) with e -> print_char first_char; failwith "Help me" in
      let (head, tail) = extract_substring_number line in
      let number = int_of_string head in
      if 0 = String.length tail then
        N(number) :: []
      else
        let () = assert (',' = (String.get tail 0) && (1 < String.length tail)) in
        let tail_without_comma = String.sub tail 1 (String.length tail - 1) in
        N(number) :: int_or_list_list_of_string tail_without_comma

        
let rec compare_iol (left_l) (right_l) :int =
        (* lexographical order <= *)
  match (left_l, right_l) with
  | (lft, rght) when lft = rght -> 0
  | ([], _) -> 1
  | ( _ :: _ , []) -> -1
  | (N left :: tail_l,
     N right :: tail_r) when left != right -> compare left right
  | (N left :: tail_l,
     N right :: tail_r) (*when left = right*) -> compare_iol tail_l tail_r
  | (L(iner_l) :: tail_l,
     L(iner_r) :: tail_r) when iner_l != iner_r -> compare_iol iner_l iner_r
  | (L(iner_l) :: tail_l,
     L(iner_r) :: tail_r) (*when iner_l = iner_r*) -> compare_iol tail_l tail_r
  | (L(iner_l) :: tail_l, 
     N(right) :: tail_r) -> compare_iol iner_l (N(right) :: [])
  | (N(left):: tail_l,
     L(iner_r) :: tail_r) -> compare_iol (N(left) :: []) iner_r

module IntOrListList_set =
Set.Make (struct type t = int_or_list list
          let compare = compare_iol
end);;

let is_sorted arr : bool = 
  if 2 < Array.length arr then
    true
  else
    let res = ref true in
    for i = 0 to (Array.length arr) - 1  do
     for j = i + 1 to (Array.length arr) do
      res := !res || arr.(i) <= arr.(j)
     done
    done;
    !res

let rm_empty_lines (lst : string list) : string list =
  let rec rm_empty_lines_rec (lst : string list) : string list =
  match lst with
  | [] -> []
  | head :: tail    when 0 = String.length head -> rm_empty_lines_rec tail
  | head :: tail (* when 0 != length *) ->  head :: rm_empty_lines_rec tail
  in rm_empty_lines_rec lst

let to_iol (lst : string list) : ((int_or_list list) * bool) list =
  let rec to_iol_rec (lst : string list) : ((int_or_list list) * bool) list =
    match lst with
    | [] -> []
    | head :: tail -> (int_or_list_list_of_string head, false) :: (to_iol_rec tail) 
  in List.rev (to_iol_rec lst)

let () =
  let lines = (read_file input_fl_name) in
  let lines2 = (rm_empty_lines lines) in
  let lists = ([L[L[N 2]]], true) :: ([L[L[N 6]]], true) :: (to_iol lines2) in
  let lists_ar = Array.of_list (lists) in

  Array.stable_sort compare lists_ar;
  
  let res = ref 1 in
  let res2 = ref 0 in
  for i = 1 to (Array.length lists_ar) do
    let j = i-1 in
    let (l, bl) = lists_ar.(j) in
    if bl then begin
      printf "%d\n" i;
      flush_all () ;
      res := !res * i;
      res2 := !res2 +1;
    end
  done;
  
  printf "%d\n" !res;;
  flush_all ()
  (* !res2 *)
 
  (*
let lines = (read_file input_fl_name)
let lines2 = (rm_empty_lines lines)
let lists = ([L[L[N 2]]], true) :: ([L[L[N 6]]], true) :: (to_iol lines2)
let lists_ar = Array.of_list (lists)  

let lines = (read_file input_fl_name)
let lines2 = List.rev (rm_empty_lines lines)
let lists = (to_iol lines2)
let lists_ar = Array.of_list (lists)
  *)