module Branch_predictor
(
    clk_i, 
    rst_i,

    update_i,
	result_i,
	predict_o
);

input            clk_i;
input            rst_i;
input            update_i;
input            result_i;
output           predict_o;
reg     [1:0]    state;

assign predict_o = state[1];

always@(posedge clk_i or posedge rst_i) begin
    if(rst_i) begin
        state <= 2'b11;
    end
    else begin
        if(update_i) begin
            if(result_i) begin
                state <= state == 2'b11 ? 2'b11 :
                            state == 2'b10 ? 2'b11 :
                            state == 2'b01 ? 2'b10 : 2'b01;  
            end
            else begin
                state <= state == 2'b00 ? 2'b00 :
                            state == 2'b01 ? 2'b00 :
                            state == 2'b10 ? 2'b01 : 2'b10;         
            end 
        end
    end
end

endmodule
