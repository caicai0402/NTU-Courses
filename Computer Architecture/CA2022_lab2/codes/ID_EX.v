module ID_EX
(
    clk_i,
    rst_i,
    flush_i,

    RegWrite_i,
    MemtoReg_i,
    MemRead_i,
    MemWrite_i,
    ALUOp_i,
    ALUSrc_i,
    Branch_i,

    RS1data_i,
    RS2data_i,
    imm_i,
    instr_i,
    pc_i,

    RegWrite_o,
    MemtoReg_o,
    MemRead_o,
    MemWrite_o,
    ALUOp_o,
    ALUSrc_o,
    Branch_o,

    RS1data_o,
    RS2data_o,
    imm_o,
    instr_o,
    pc_o
);

input            clk_i;
input            rst_i;

input            flush_i;
input            RegWrite_i;
input            MemtoReg_i;
input            MemRead_i;
input            MemWrite_i;
input   [1:0]    ALUOp_i;
input            ALUSrc_i;
input            Branch_i;

input   [31:0]   RS1data_i;
input   [31:0]   RS2data_i;
input   [31:0]   imm_i;
input   [31:0]   instr_i;
input   [31:0]   pc_i;

output           RegWrite_o;
output           MemtoReg_o;
output           MemRead_o;
output           MemWrite_o;
output  [1:0]    ALUOp_o;
output           ALUSrc_o;
output           Branch_o;

output  [31:0]   RS1data_o;
output  [31:0]   RS2data_o;
output  [31:0]   imm_o;
output  [31:0]   instr_o;
output  [31:0]   pc_o;

reg              RegWrite_o;
reg              MemtoReg_o;
reg              MemRead_o;
reg              MemWrite_o;
reg     [1:0]    ALUOp_o;
reg              ALUSrc_o;
reg              Branch_o;

reg     [31:0]   RS1data_o;
reg     [31:0]   RS2data_o;
reg     [31:0]   imm_o;
reg     [31:0]   instr_o;
reg     [31:0]   pc_o;

always@(posedge clk_i or posedge rst_i) begin
    if(rst_i) begin
        RegWrite_o <= 1'b0;
        MemtoReg_o <= 1'b0;
        MemRead_o <= 1'b0;
        MemWrite_o <= 1'b0;
        ALUOp_o <= 2'b0;
        ALUSrc_o <= 1'b0;
        Branch_o <= 1'b0;
        RS1data_o <= 32'b0;
        RS2data_o <= 32'b0;
        imm_o <= 32'b0;
        instr_o <= 32'b0;
        pc_o <= 32'b0;
    end 
    else begin
        RegWrite_o <= flush_i? 1'b0 : RegWrite_i;
        MemtoReg_o <= flush_i? 1'b0 : MemtoReg_i;
        MemRead_o <= flush_i? 1'b0 : MemRead_i;
        MemWrite_o <= flush_i? 1'b0 : MemWrite_i;
        ALUOp_o <= flush_i? 2'b0 : ALUOp_i;
        ALUSrc_o <= flush_i? 1'b0 : ALUSrc_i;
        Branch_o <= flush_i? 1'b0 : Branch_i;
        RS1data_o <= flush_i? 32'b0 : RS1data_i;
        RS2data_o <= flush_i? 32'b0 : RS2data_i;
        imm_o <= flush_i? 32'b0 : imm_i;
        instr_o <= flush_i? 32'b0 : instr_i;
        pc_o <= flush_i? 32'b0 : pc_i;
    end
end

endmodule