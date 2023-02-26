module EX_MEM
(
    clk_i,
    rst_i,
    RegWrite_i,
    MemtoReg_i,
    MemRead_i,
    MemWrite_i,
    ALURet_i,
    WriteData_i,
    RDaddr_i,

    RegWrite_o,
    MemtoReg_o,
    MemRead_o,
    MemWrite_o,
    ALURet_o,
    WriteData_o,
    RDaddr_o,
);

input            clk_i;
input            rst_i;
input            RegWrite_i;
input            MemtoReg_i;
input            MemRead_i;
input            MemWrite_i;
input   [31:0]   ALURet_i;
input   [31:0]   WriteData_i;
input   [4:0]    RDaddr_i;

output           RegWrite_o;
output           MemtoReg_o;
output           MemRead_o;
output           MemWrite_o;
output  [31:0]   ALURet_o;
output  [31:0]   WriteData_o;
output  [4:0]    RDaddr_o;

reg              RegWrite_o;
reg              MemtoReg_o;
reg              MemRead_o;
reg              MemWrite_o;
reg     [31:0]   ALURet_o;
reg     [31:0]   WriteData_o;
reg     [4:0]    RDaddr_o;

always@(posedge clk_i or posedge rst_i) begin
    if(rst_i) begin
        RegWrite_o <= 1'b0;
        MemtoReg_o <= 1'b0;
        MemRead_o <= 1'b0;
        MemWrite_o <= 1'b0;
        ALURet_o <= 32'b0;
        WriteData_o <= 32'b0;
        RDaddr_o <= 5'b0;
    end  
    else begin
        RegWrite_o <= RegWrite_i;
        MemtoReg_o <= MemtoReg_i;
        MemRead_o <= MemRead_i;
        MemWrite_o <= MemWrite_i;
        ALURet_o <= ALURet_i;
        WriteData_o <= WriteData_i;
        RDaddr_o <= RDaddr_i;
    end
end

endmodule