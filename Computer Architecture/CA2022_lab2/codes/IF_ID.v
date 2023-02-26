module IF_ID
(
    clk_i,
    rst_i,
    flush_i,
    stall_i,
    instr_i,
    pc_i,    
    instr_o,
    pc_o,
);

input            clk_i;
input            rst_i;
input            flush_i;
input            stall_i;
input   [31:0]   instr_i;
input   [31:0]   pc_i;
output  [31:0]   instr_o;
output  [31:0]   pc_o;

reg     [31:0]   instr_o;
reg     [31:0]   pc_o;

always@(posedge clk_i or posedge rst_i) begin
    if(rst_i) begin
        pc_o <= 32'b0;
        instr_o <= 32'b0;
    end
    else begin
        if(!stall_i) begin
            pc_o <= pc_i;
            instr_o <= flush_i? 32'b0 : instr_i;
        end
        else begin
            pc_o <= pc_o;
            instr_o <= instr_o;
        end
    end
end

endmodule