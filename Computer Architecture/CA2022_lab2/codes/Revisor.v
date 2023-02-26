module Revisor
(
   Branch_i,
   Zero_i,
   Predict_i,
   flush_o
);

input            Branch_i;
input            Zero_i;
input            Predict_i;
output           flush_o;

assign flush_o = Branch_i? (Zero_i != Predict_i ? 1 : 0) : 0;  

endmodule