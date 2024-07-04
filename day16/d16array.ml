
let input_fl_name = "in2.txt"

let read_file filename = 
  let lines = ref [] in
  let chan = open_in filename in
  try
    while true; do
      lines := ((input_line chan)) :: !lines
    done; !lines
  with End_of_file ->
    close_in chan;
    List.rev !lines;;

let num_lines = 
  let lines = read_file input_fl_name in
  List.length lines

let lines = (read_file input_fl_name)
let lines_ar = Array.of_list (lines)
let n_of_lines = num_lines
  

type valve = {flow_rate :int; tunnels: int list}
let dvlv = {flow_rate = 0; tunnels =[]}

module Valves =
Set.Make (struct type t = valve 
          let compare = fun v1 -> fun v2 -> compare v1.flow_rate v2.flow_rate
end);;

let create_letercode_conversion (lines :string list) =
  let res = Hashtbl.create 10 in
  let codes = List.map (fun (line) -> Scanf.sscanf line "Valve %c%c" (fun (c1 ) -> fun c2 -> (c1, c2))) lines in
  let i = ref 0 in
  List.iter (fun (a, b) -> Hashtbl.add res (a, b) !i; i := !i + 1) codes;
  res

let code_table =
  let unsorted_lines = List.map (fun (line) -> (Scanf.sscanf line "Valve %c%c has flow rate=%d" (fun _ -> fun _ -> fun d -> d)), line) lines in
  let sorted_lines_meta = List.sort (fun (a, b) -> fun (a', b') -> compare a a') unsorted_lines in
  let sorted_lines = List.map (fun (a, b) -> b) sorted_lines_meta in
  create_letercode_conversion (List.rev sorted_lines)

let code_to_int c1 c2 : int = 
  Hashtbl.find code_table (c1, c2)
  

let text_to_valve (line :string) : (int * valve) =
  let lst = String.split_on_char ';' line in
  let first_half = List.hd lst in
  let secnd_half = List.hd (List.tl lst) in
  let tunnels_list_txt = String.sub secnd_half 23 (* tunnels lead to valves *) (-23 + String.length secnd_half) in
  let tunnel_txt_lst = String.split_on_char ',' tunnels_list_txt in
  let tunnel_list = List.map (fun (txt) -> Scanf.sscanf txt " %c%c" (fun lsb -> fun msb -> code_to_int lsb msb)) tunnel_txt_lst in 
  let scanner (lsb : char) (msb : char) (fl_rate: int)  = 
    code_to_int lsb msb, {flow_rate = fl_rate; tunnels = tunnel_list}
  in
  Scanf.sscanf first_half "Valve %c%c has flow rate=%d" scanner

module Bitfield = struct
  type t = Int.t
  let set (bit_field : t) (idx : int) =
    (** [set bit_field n] sets the [n]th bit in [bitfield]. *)
    let mask = Int.shift_left 1 idx in
    Int.logor bit_field mask
  
  let clear (bit_field : t) (idx : int) =
    (** [clear bit_field n] clears the [n]th bit in [bitfield]. *)
    let mask = Int.lognot (Int.shift_left 1 idx) in
    Int.logand bit_field mask
  
  
  let get (bit_field : t) (idx : int ) =
    (** [get bit_field n] gets the [n]th bit in [bitfield]. *)
    let mask = Int.shift_left 1 idx in
    0 != Int.logand bit_field mask
  
  let empty = 0
end


let tunel_valve_graph = Array.make num_lines dvlv
let () =
  List.iter 
  (fun (elem) -> 
    let (idx, vlv) = text_to_valve elem in
    tunel_valve_graph.(idx) <- vlv) lines;
  Array.sort compare tunel_valve_graph


let rec list_max lst =
  match lst with
  | [] -> failwith "No max of an empty list"
  | head :: [] -> head
  | head :: tail -> max head (list_max tail)

let p2_total_time = 26

let posible_states = Array.make (123928575 +10) (-1)

let get_key (time_left : int) (position :int) (open_vaves : Bitfield.t) (is_elephant :bool) =
  Bool.to_int is_elephant +
  2 * (time_left +
  31 * (position + 
  (List.length lines) * open_vaves))
  

(* returns maximum pressure relief *)
let rec solve_d16 (time_left : int) (position :int) (pressure_released :int) (open_vaves : Bitfield.t) (is_elephant :bool) =
  let current_valve = tunel_valve_graph.(position) in
  if time_left = 0 then
    if is_elephant 
      then pressure_released
      else solve_d16 p2_total_time (code_to_int 'A' 'A') pressure_released open_vaves true
  else 
    if -1 = posible_states.(get_key time_left position open_vaves is_elephant) then 
      let posible_reliefs = List.map (fun (vlv) -> solve_d16 (time_left - 1) vlv pressure_released open_vaves is_elephant) current_valve.tunnels in
      let turn_on_valve_relief = if not (Bitfield.get open_vaves position) && current_valve.flow_rate > 0
        then let new_open_valves = Bitfield.set open_vaves position in
        solve_d16 (time_left - 1) position (pressure_released + current_valve.flow_rate* (time_left - 1)) new_open_valves is_elephant
        else 0 in
      let answer = list_max (turn_on_valve_relief :: posible_reliefs) in
      posible_states.(get_key time_left position open_vaves is_elephant) <- answer;
      answer
    else
      posible_states.(get_key time_left position open_vaves is_elephant)

let aa = tunel_valve_graph.(0) 


let () =
                                           (*AA*)
  print_string "Hello\n";
  flush_all ();
  (* part 1 *)
  print_int (solve_d16 30 (code_to_int 'A' 'A') 0 Bitfield.empty true);
  print_newline ();
  flush_all ();
  (* part 2 *)
  print_int (solve_d16 p2_total_time (code_to_int 'A' 'A') 0 Bitfield.empty false);
  flush_all ()
  


