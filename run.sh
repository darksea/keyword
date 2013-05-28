#!/bin/bash
./posfilter untitled
./section untitled_pos
./tuple untitled_pos_part 
./simMatrix untitled_pos_part_tuple
./bc simadj untitled_pos_part_tuple
./bcdensity untitled_pos_part_tuple shortpath haha
./score haha 5 > result_untitled
rm untitled_pos untitled_pos_part untitled_pos_part_tuple BetweenCencility haha kw_score shortpath simadj 
rm untitled result_untitled
