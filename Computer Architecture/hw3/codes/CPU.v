module CPU
(
    clk_i, 
    rst_i,
    start_i
);

// Ports
input               clk_i;
input               rst_i;
input               start_i;

parameter four = 4;
wire    [31:0]      pcin;
wire    [31:0]      pcout;
wire    [31:0]      addr;
wire    [31:0]      instr;
wire    [31:0]      rs1;
wire    [31:0]      rs2;
wire    [31:0]      rd;
wire    [31:0]      imm;
wire    [31:0]      mux2alu;

wire    [1:0]       ALUOp;
wire                ALUSrc;
wire                RegWrite;
wire    [2:0]       ALUCtrl;


Control Control(
    .Op_i       (instr[6:0]),
    .ALUOp_o    (ALUOp),
    .ALUSrc_o   (ALUSrc),
    .RegWrite_o (RegWrite)
);

Adder Add_PC(
    .data1_in   (pcout),
    .data2_in   (four),
    .data_o     (pcin)
);

PC PC(
    .clk_i      (clk_i),
    .rst_i      (rst_i),
    .start_i    (start_i),
    .pc_i       (pcin),
    .pc_o       (pcout)
);

Instruction_Memory Instruction_Memory(
    .addr_i     (pcout), 
    .instr_o    (instr)
);

Registers Registers(
    .clk_i      (clk_i),
    .RS1addr_i  (instr[19:15]),
    .RS2addr_i  (instr[24:20]),
    .RDaddr_i   (instr[11:7]), 
    .RDdata_i   (rd),
    .RegWrite_i (RegWrite), 
    .RS1data_o  (rs1), 
    .RS2data_o  (rs2) 
);

MUX32 MUX_ALUSrc(
    .data1_i    (rs2),
    .data2_i    (imm),
    .select_i   (ALUSrc),
    .data_o     (mux2alu)
);

Sign_Extend Sign_Extend(
    .data_i     (instr[31:20]),
    .data_o     (imm)
);
  
ALU ALU(
    .data1_i    (rs1),
    .data2_i    (mux2alu),
    .ALUCtrl_i  (ALUCtrl),
    .data_o     (rd),
    .Zero_o     ()
);

ALU_Control ALU_Control(
    .funct_i    ({instr[31:25], instr[14:12]}),
    .ALUOp_i    (ALUOp),
    .ALUCtrl_o  (ALUCtrl)
);

endmodule

