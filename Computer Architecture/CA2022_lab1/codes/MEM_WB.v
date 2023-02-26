module MEM_WB
(
    clk_i,
    rst_i,
    RegWrite_i,
    MemtoReg_i,
    ALURet_i,
    ReadData_i,
    RDaddr_i,

    RegWrite_o,
    MemtoReg_o,
    ALURet_o,
    ReadData_o,
    RDaddr_o,
);

// Ports
input               clk_i;
input               rst_i;
input               RegWrite_i;
input               MemtoReg_i;
input   [31:0]      ALURet_i;
input   [31:0]      ReadData_i;
input   [4:0]       RDaddr_i;

output              RegWrite_o;
output              MemtoReg_o;
output  [31:0]      ALURet_o;
output  [31:0]      ReadData_o;
output  [4:0]       RDaddr_o;

// Wires & Registers
reg                 RegWrite_o;
reg                 MemtoReg_o;
reg     [31:0]      ALURet_o;
reg     [31:0]      ReadData_o;
reg     [4:0]       RDaddr_o;

always@(posedge clk_i or posedge rst_i) begin
    if(rst_i) begin
        RegWrite_o <= 1'b0;
        MemtoReg_o <= 1'b0;
        ALURet_o <= 32'b0;
        ReadData_o <= 32'b0;
        RDaddr_o <= 5'b0;
    end  
    else begin
        RegWrite_o <= RegWrite_i;
        MemtoReg_o <= MemtoReg_i;
        ALURet_o <= ALURet_i;
        ReadData_o <= ReadData_i;
        RDaddr_o <= RDaddr_i;
    end
end

endmodule