type cords = {x : int; y : int} 
let input_fl_name = "in.txt"

let read_file filename = 
  let lines = ref [] in
  let chan = open_in filename in
  try
    while true; do
      lines := (String.to_bytes (input_line chan)) :: !lines
    done; !lines
  with End_of_file ->
    close_in chan;
    List.rev !lines ;;



  let lines = Array.of_list (read_file input_fl_name) in
  let height = Array.length lines in 
  let width =  Bytes.length lines.(0) in
  let height_map = (Array.make_matrix width height 0 ) in
  let me : cords ref = ref {x= 0; y =0} in
  let end_goal : cords ref = ref {x= 0; y =0} in
  for x = 0 to width-1 do
    for y = 0 to height-1 do
      let c =(Bytes.get lines.(y) x) in
      if (Char.uppercase_ascii c) = c then
        match c with
        |'S' -> height_map.(x).(y) <- Char.code 'z' - Char.code 'a';
        me :=  {x= x; y= y}
        |'E' -> height_map.(x).(y) <- Char.code 'z' - Char.code 'a';
        end_goal := {x= x; y= y}
        | _ -> failwith "Not valid input"
      else
        height_map.(x).(y) <- Char.code c - Char.code 'a';
        
      done; 
    done;
    Printf.printf "end goal (%d, %d) \n" !end_goal.x !end_goal.y;
    Printf.printf "Start:   (%d, %d) \n" !me.x !me.y;
    
    for y = 0 to height-1 do
      for x = 0 to width-1 do
        let c =(Bytes.get lines.(y) x) in
        print_char c;
      done; print_char '\n'
    done; print_char '\n';
    flush_all ();
    
    let is_in_bounds (pos: cords) : bool = 
      pos.x >= 0 && pos.x < width &&
      pos.y >= 0 && pos.y < height 
    in
    let shortest_path (start:cords) =
      let (visited_map : bool array array) = (Array.make_matrix width height false) in
      let (bfs_queue: (cords * int * cords list) Queue.t) = Queue.create () in
      let ans = ref (0, []) in
      Queue.add (start, 0, start::[]) bfs_queue;
      while not visited_map.(!end_goal.x).(!end_goal.y) && not (Queue.is_empty bfs_queue) do
        let (current_pos, path_length, path) = Queue.take bfs_queue in
        if not visited_map.(current_pos.x).(current_pos.y) then begin
          visited_map.(current_pos.x).(current_pos.y) <- true;
          if current_pos = !end_goal then
            ans := path_length, path;
        let next_positions = {x = current_pos.x +1; y =current_pos.y} :: {x = current_pos.x -1; y =current_pos.y}:: {x = current_pos.x; y =current_pos.y +1}:: {x = current_pos.x; y =current_pos.y -1}:: [] in
        let iterate (position : cords) :unit = 
          if (is_in_bounds position) && 
             (not visited_map.(position.x).(position.y)) &&
             (height_map.(position.x).(position.y) -1 <= (height_map.(current_pos.x).(current_pos.y))) then
            Queue.add (position, path_length+1, position::path) bfs_queue
          else
            ()
             (* ; Printf.printf "(%d, %d)" position.x position.y; flush_all ()
             
            ;print_int (Bool.to_int (is_in_bounds position));
            if is_in_bounds position then begin
              print_int (Bool.to_int (not visited_map.(position.x).(position.y)));
              print_int (Bool.to_int (height_map.(position.x).(position.y) <= height_map.(current_pos.x).(current_pos.y)))
            end;
         
            print_char '\n'; *)
        in
        List.iter (iterate) next_positions;
      end
    done;
    if((Queue.is_empty bfs_queue)) then
      (100000, [])
    else
      !ans in
  
  let (len, pth) = (123123, []) in
  let print_node (pos : cords) = 
    Printf.printf " (%d, %d) - \n" pos.x pos.y
  in
  List.iter (print_node) (List.rev pth);
  print_int len;
  print_string "\naaaaaaa\n\n\n";
  flush_all ();


  let minimum (a:int) b =
    if a < b then a else b in
  let min_path = ref [] in
  let min_path_len = ref len in
  let min_path_start = ref !me in
  for x = 0 to width-1 do 
    for y = 0 to height-1 do
      if (Bytes.get lines.(y) x) = 'a' then begin
        let (lenth, pathhh) = shortest_path {x = x; y = y} in 
        if lenth < !min_path_len then begin
          min_path := pathhh;
          min_path_len := lenth;
          min_path_start := {x = x; y = y};
        end;
        print_int lenth;
        print_node {x = x; y = y}
      end
    done
  done;
  (* List.iter (print_node) (List.rev !min_path); *)
  print_int !min_path_len;
  print_node !min_path_start;
  flush_all () ;

