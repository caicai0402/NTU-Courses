module ID_EX
(
    clk_i,
    rst_i,
    RegWrite_i,
    MemtoReg_i,
    MemRead_i,
    MemWrite_i,
    ALUOp_i,
    ALUSrc_i,
    RS1data_i,
    RS2data_i,
    imm_i,
    instr_i,

    RegWrite_o,
    MemtoReg_o,
    MemRead_o,
    MemWrite_o,
    ALUOp_o,
    ALUSrc_o,
    RS1data_o,
    RS2data_o,
    imm_o,
    instr_o,
);

// Ports
input               clk_i;
input               rst_i;
input               RegWrite_i;
input               MemtoReg_i;
input               MemRead_i;
input               MemWrite_i;
input   [1:0]       ALUOp_i;
input               ALUSrc_i;
input   [31:0]      RS1data_i;
input   [31:0]      RS2data_i;
input   [31:0]      imm_i;
input   [31:0]      instr_i;

output              RegWrite_o;
output              MemtoReg_o;
output              MemRead_o;
output              MemWrite_o;
output  [1:0]       ALUOp_o;
output              ALUSrc_o;
output  [31:0]      RS1data_o;
output  [31:0]      RS2data_o;
output  [31:0]      imm_o;
output  [31:0]      instr_o;

// Wires & Registers
reg                 RegWrite_o;
reg                 MemtoReg_o;
reg                 MemRead_o;
reg                 MemWrite_o;
reg     [1:0]       ALUOp_o;
reg                 ALUSrc_o;
reg     [31:0]      RS1data_o;
reg     [31:0]      RS2data_o;
reg     [31:0]      imm_o;
reg     [31:0]      instr_o;

always@(posedge clk_i or posedge rst_i) begin
    if(rst_i) begin
        RegWrite_o <= 1'b0;
        MemtoReg_o <= 1'b0;
        MemRead_o <= 1'b0;
        MemWrite_o <= 1'b0;
        ALUOp_o <= 2'b0;
        ALUSrc_o <= 1'b0;
        RS1data_o <= 32'b0;
        RS2data_o <= 32'b0;
        imm_o <= 32'b0;
        instr_o <= 32'b0;
    end 
    else begin
        RegWrite_o <= RegWrite_i;
        MemtoReg_o <= MemtoReg_i;
        MemRead_o <= MemRead_i;
        MemWrite_o <= MemWrite_i;
        ALUOp_o <= ALUOp_i;
        ALUSrc_o <= ALUSrc_i;
        RS1data_o <= RS1data_i;
        RS2data_o <= RS2data_i;
        imm_o <= imm_i;
        instr_o <= instr_i;
    end
end

endmodule