module Flusher
(
    RS1data_i,
    RS2data_i,
    Branch_i,
    Flush_o
);

// Ports
input   [31:0]      RS1data_i;
input   [31:0]      RS2data_i;
input               Branch_i;
output              Flush_o;

assign Flush_o = (RS1data_i == RS2data_i && Branch_i) ? 1 : 0;

endmodule
