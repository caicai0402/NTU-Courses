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

// Constant
parameter four = 4;


wire    [31:0]      IF_pc;
wire    [31:0]      IF_pc_new;
wire    [31:0]      IF_pc_4;
wire    [31:0]      IF_pc_imm;
wire    [31:0]      IF_instr;

wire                PCWrite;
wire                Stall;
wire                Flush;
wire                NoOp;

wire                ID_RegWrite;
wire                ID_MemtoReg;
wire                ID_MemRead;
wire                ID_MemWrite;
wire    [1:0]       ID_ALUOp;
wire                ID_ALUSrc;
wire                Branch;

wire    [31:0]      ID_pc;
wire    [31:0]      ID_RS1data;
wire    [31:0]      ID_RS2data;
wire    [31:0]      ID_IMM;
wire    [31:0]      ID_instr;

wire                EX_RegWrite;
wire                EX_MemtoReg;
wire                EX_MemRead;
wire                EX_MemWrite;
wire    [1:0]       EX_ALUOp;
wire                EX_ALUSrc;

wire    [31:0]      EX_RS1data;
wire    [31:0]      EX_RS2data;
wire    [31:0]      EX_IMM;
wire    [31:0]      EX_instr;

wire    [31:0]      ALUSrc1;
wire    [31:0]      ALUSrc2;
wire    [31:0]      EX_WriteData;
wire    [31:0]      EX_ALURet;
wire    [2:0]       ALUCtrl;

wire                MEM_RegWrite;
wire                MEM_MemtoReg;
wire                MEM_MemRead;
wire                MEM_MemWrite;
wire    [31:0]      MEM_ALURet;
wire    [31:0]      MEM_WriteData;
wire    [4:0]       MEM_RDaddr;
wire    [31:0]      MEM_MemReadData;

wire                WB_RegWrite;
wire                WB_MemtoReg;
wire    [31:0]      WB_ALURet;
wire    [31:0]      WB_MemReadData;
wire    [4:0]       WB_RDaddr;
wire    [31:0]      WB_RDdata;

wire    [1:0]       ForwardA;
wire    [1:0]       ForwardB;


PC PC(
    .clk_i          (clk_i),
    .rst_i          (rst_i),
    .start_i        (start_i), 
    .PCWrite_i      (PCWrite),
    .pc_i           (IF_pc_new),
    .pc_o           (IF_pc)
);

Adder Add_4(
    .data1_in       (IF_pc),
    .data2_in       (four),
    .data_o         (IF_pc_4)
);

Adder Add_imm(
    .data1_in       (ID_pc),
    .data2_in       (ID_IMM),
    .data_o         (IF_pc_imm)
);

MUX2 MUX2_PC(
    .data0_i        (IF_pc_4),
    .data1_i        (IF_pc_imm),
    .select_i       (Flush),
    .data_o         (IF_pc_new)
);

Instruction_Memory Instruction_Memory(
    .addr_i         (IF_pc), 
    .instr_o        (IF_instr)
);

Registers Registers(
    .clk_i          (clk_i),
    .RS1addr_i      (ID_instr[19:15]),
    .RS2addr_i      (ID_instr[24:20]),
    .RDaddr_i       (WB_RDaddr), 
    .RDdata_i       (WB_RDdata),
    .RegWrite_i     (WB_RegWrite), 
    .RS1data_o      (ID_RS1data), 
    .RS2data_o      (ID_RS2data) 
);

IMM_GEN IMM_GEN(
    .data_i         (ID_instr),
    .data_o         (ID_IMM)
);

Control Control(
    .Op_i           (ID_instr[6:0]),
    .NoOp_i         (NoOp),
    .RegWrite_o     (ID_RegWrite),
    .MemtoReg_o     (ID_MemtoReg),
    .MemRead_o      (ID_MemRead),
    .MemWrite_o     (ID_MemWrite),
    .ALUOp_o        (ID_ALUOp),
    .ALUSrc_o       (ID_ALUSrc),
    .Branch_o       (Branch)
);

Flusher Flusher(
    .RS1data_i     (ID_RS1data),
    .RS2data_i     (ID_RS2data),
    .Branch_i      (Branch),
    .Flush_o       (Flush)
);

MUX4 MUX_FA(
    .data0_i        (EX_RS1data),
    .data1_i        (WB_RDdata),
    .data2_i        (MEM_ALURet),
    .data3_i        (),
    .select_i       (ForwardA),
    .data_o         (ALUSrc1)
);

MUX4 MUX_FB(
    .data0_i        (EX_RS2data),
    .data1_i        (WB_RDdata),
    .data2_i        (MEM_ALURet),
    .data3_i        (),
    .select_i       (ForwardB),
    .data_o         (EX_WriteData)
);

MUX2 MUX_ALUSrc(
    .data0_i        (EX_WriteData),
    .data1_i        (EX_IMM),
    .select_i       (EX_ALUSrc),
    .data_o         (ALUSrc2)
);

ALU ALU(
    .data1_i        (ALUSrc1),
    .data2_i        (ALUSrc2),
    .ALUCtrl_i      (ALUCtrl),
    .data_o         (EX_ALURet)
);

ALU_Control ALU_Control(
    .funct_i        ({EX_instr[31:25], EX_instr[14:12]}),
    .ALUOp_i        (EX_ALUOp),
    .ALUCtrl_o      (ALUCtrl)
);

Data_Memory Data_Memory(
    .clk_i          (clk_i),
    .addr_i         (MEM_ALURet),
    .MemRead_i      (MEM_MemRead),
    .MemWrite_i     (MEM_MemWrite),
    .data_i         (MEM_WriteData),
    .data_o         (MEM_MemReadData)
);

MUX2 MUX2_WB(
    .data0_i        (WB_ALURet),
    .data1_i        (WB_MemReadData),
    .select_i       (WB_MemtoReg),
    .data_o         (WB_RDdata)
);

Hazard_Detection_Unit Hazard_Detection_Unit(
    .ID_RS1addr_i   (ID_instr[19:15]),
    .ID_RS2addr_i   (ID_instr[24:20]),
    .EX_RDaddr_i    (EX_instr[11:7]),
    .EX_MemRead_i   (EX_MemRead),
    .PCWrite_o      (PCWrite),
    .Stall_o        (Stall),
    .NoOp_o         (NoOp)
);

Forwarding_Unit Forwarding_Unit(
    .EX_RS1addr_i   (EX_instr[19:15]),
    .EX_RS2addr_i   (EX_instr[24:20]),
    .MEM_RegWrite_i (MEM_RegWrite),
    .MEM_RDaddr_i   (MEM_RDaddr),
    .WB_RegWrite_i  (WB_RegWrite),
    .WB_RDaddr_i    (WB_RDaddr),
    .ForwardA_o     (ForwardA),
    .ForwardB_o     (ForwardB)
);

IF_ID IF_ID(
    .clk_i          (clk_i),
    .rst_i          (rst_i),
    .pc_i           (IF_pc),
    .instr_i        (IF_instr),
    .stall_i        (Stall),
    .flush_i        (Flush),
    .pc_o           (ID_pc),
    .instr_o        (ID_instr)
);

ID_EX ID_EX(
    .clk_i          (clk_i),
    .rst_i          (rst_i),
    .RegWrite_i     (ID_RegWrite),
    .MemtoReg_i     (ID_MemtoReg),
    .MemRead_i      (ID_MemRead),
    .MemWrite_i     (ID_MemWrite),
    .ALUOp_i        (ID_ALUOp),
    .ALUSrc_i       (ID_ALUSrc),
    .RS1data_i      (ID_RS1data),
    .RS2data_i      (ID_RS2data),
    .imm_i          (ID_IMM),
    .instr_i        (ID_instr),
    .RegWrite_o     (EX_RegWrite),
    .MemtoReg_o     (EX_MemtoReg),
    .MemRead_o      (EX_MemRead),
    .MemWrite_o     (EX_MemWrite),
    .ALUOp_o        (EX_ALUOp),
    .ALUSrc_o       (EX_ALUSrc),
    .RS1data_o      (EX_RS1data),
    .RS2data_o      (EX_RS2data),
    .imm_o          (EX_IMM),
    .instr_o        (EX_instr)
);

EX_MEM EX_MEM(
    .clk_i          (clk_i),
    .rst_i          (rst_i),
    .RegWrite_i     (EX_RegWrite),
    .MemtoReg_i     (EX_MemtoReg),
    .MemRead_i      (EX_MemRead),
    .MemWrite_i     (EX_MemWrite),
    .ALURet_i       (EX_ALURet),
    .WriteData_i    (EX_WriteData),
    .RDaddr_i       (EX_instr[11:7]),
    .RegWrite_o     (MEM_RegWrite),
    .MemtoReg_o     (MEM_MemtoReg),
    .MemRead_o      (MEM_MemRead),
    .MemWrite_o     (MEM_MemWrite),
    .ALURet_o       (MEM_ALURet),
    .WriteData_o    (MEM_WriteData),
    .RDaddr_o       (MEM_RDaddr)
);

MEM_WB MEM_WB(
    .clk_i          (clk_i),
    .rst_i          (rst_i),
    .RegWrite_i     (MEM_RegWrite),
    .MemtoReg_i     (MEM_MemtoReg),
    .ALURet_i       (MEM_ALURet),
    .ReadData_i     (MEM_MemReadData),
    .RDaddr_i       (MEM_RDaddr),
    .RegWrite_o     (WB_RegWrite),
    .MemtoReg_o     (WB_MemtoReg),
    .ALURet_o       (WB_ALURet),
    .ReadData_o     (WB_MemReadData),
    .RDaddr_o       (WB_RDaddr)
);

endmodule