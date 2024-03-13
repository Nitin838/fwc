module helloworldfpga(output reg LCD_RS,output reg LCD_E,output reg[7:4] DATA,output wire redled, output wire greenled, output wire blueled);

wire clk;
qlal4s3b_cell_macro u_qlal4s3b_cell_macro (
    .Sys_Clk0 (clk),
);//20Mhz clock
integer i = 1;
reg [25:0] count=0;
reg [3:0] Datas [1:37];
reg [1:0] state;
reg [26:0] delay_counter; // Delay counter

always @(posedge clk) begin
Datas[1]   =  4'h3;     //-- initializing controller--
Datas[2]   =  4'h3;   
Datas[3]   =  4'h3;     //-- set to 4-bit input mode --
Datas[4]   =  4'h2;     
Datas[5]   =  4'h2;     //--2 line, 5x7 matrix  --
Datas[6]   =  4'h8;     
Datas[7]   =  4'h0;     //--turn cursor off (0x0E to enable) --
Datas[8]   =  4'hC;     
Datas[9]   =  4'h0;     //-- cursor direction = right --
Datas[10]  =  4'h6;     
Datas[11]  =  4'h0;     //--  start with clear display  --
Datas[12]  =  4'h1;
Datas[13]  =  4'h8; //starting from line 1
Datas[14]  =  4'h0;
Datas[15]  =  1'b1;
Datas[16]  =  4'h4;//A
Datas[17]  =  4'h1;   
Datas[18]  =  4'h4;//N==0X48
Datas[19]  =  4'hE;
Datas[20]  =  4'h5;//S=0X45;       
Datas[21]  =  4'h3;
Datas[22]  =  4'h5;//W
Datas[23]  =  4'h7;
Datas[24]  =  4'h4;//E
Datas[25]  =  4'h5;
Datas[26]  =  4'h5;//R
Datas[27]  =  4'h2;
Datas[28]  =  4'h3;//=
Datas[29]  =  4'hD;
Datas[30]  =  4'h3;//3
Datas[31]  =  4'h3;
Datas[32]  =  4'h3;//0
Datas[33]  =  4'h0;
Datas[34]  =  4'h2;//%
Datas[35]  =  4'h5;
Datas[36]  =  4'h2;//space=0x20
Datas[37]  =  4'h0;
//Datas[36]  =  4'h3;//3=0x33
//Datas[37]  =  4'h3;
//Datas[38]  =  4'h3;//0=30
//Datas[39]  =  4'h0;
//Datas[40]  =  4'h2;//%=0x25
//Datas[41]  =  4'h5;
end
always @(posedge clk) begin
if (i<=14)begin
    LCD_RS<=1'b0;
    DATA=Datas[i];
    LCD_E<=1'b1;
    if (count==800)begin //waiting 40us
        LCD_E<=1'b0;
        count<=0;
        i <= i + 1;
        end
    else
        count<=count+1;

    end
if (i==15) begin
    if (count==60000)begin//waiting 3ms
            count<=0;
            i<=i+1;
            end
    else
            count<=count+1;
    end
if (i>15 & i<=37) begin 
    LCD_RS<=1'b1;
    DATA=Datas[i];
        LCD_E<=1'b1;
    if (count==800)begin //waiting 40us
        LCD_E<=1'b0;
        count<=0;
            i <= i + 1;
            end
        else
        count<=count+1;       
        end
if (i>37) 
    i<=13;
    
end
always @(posedge clk) begin
    if (delay_counter == 20000000) begin // Adjust delay value as needed
        delay_counter <= 0;
        state <= state + 1; // Increment state after delay
    end else begin
        delay_counter <= delay_counter + 1; // Increment delay counter
    end
end
assign redled = state[1];
assign blueled = state[0];

assign greenled = redled & blueled;

endmodule