
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity project_reti_logiche is
    Port (
        i_clk : in std_logic;
        i_rst : in std_logic;
        i_start : in std_logic;
        i_data : in std_logic_vector(7 downto 0);
        o_address : out std_logic_vector(15 downto 0);
        o_done : out std_logic;
        o_en : out std_logic;
        o_we : out std_logic;
        o_data : out std_logic_vector (7 downto 0)
     );
end project_reti_logiche;

architecture project_reti_logiche of project_reti_logiche is

component datapath is 
    Port(
         i_clk : in STD_LOGIC;
         i_rst : in STD_LOGIC;
         i_data : in std_logic_vector(7 downto 0);
         o_data : out std_logic_vector (7 downto 0);
         o_address : out std_logic_vector(15 downto 0);
         -- custom signals
         num_sel : in std_logic;
         num_load : in std_logic;
         p1_en : in std_logic;
         p2_en : in std_logic;
         out_sel : in std_logic;
         out_en : in std_logic;
         ra_load : in std_logic;
         ra_sel : in std_logic;
         wa_load : in std_logic;
         wa_sel : in std_logic;
         rw_sel : in std_logic;
         last_sel : in std_logic;
         last_load : in std_logic;
         cur_load : in std_logic;
         o_end : out std_logic
    );
end component;
signal num_sel : std_logic;
signal num_load : std_logic;
signal p1_en : std_logic;
signal p2_en : std_logic;
signal out_sel : std_logic;
signal out_en : std_logic;
signal ra_load : std_logic;
signal ra_sel : std_logic;
signal wa_load : std_logic;
signal wa_sel : std_logic;
signal rw_sel : std_logic;
signal last_sel : std_logic;
signal last_load : std_logic;
signal cur_load : std_logic;
signal o_end : std_logic;
type S is (S0, S1, S2, S3, S4, S44, S45, S5, S6, S7, S8);
signal cur_state, next_state : S; 
begin
    DATAPATH0: datapath port map(
        i_clk,
        i_rst,
        i_data,
        o_data,
        o_address,
        num_sel,
        num_load,
        p1_en,
        p2_en,
        out_sel,
        out_en,
        ra_load,
        ra_sel,
        wa_load,
        wa_sel,
        rw_sel,
        last_sel,
        last_load,
        cur_load,
        o_end
    );
    
    process(i_clk, i_rst)
    begin
        if(i_rst = '1') then
            cur_state <= S0;
        elsif i_clk'event and i_clk = '1' then
            cur_state <= next_state;
        end if;
    end process;    
    
    process(cur_state, i_start, o_end)   
    begin
        next_state <= cur_state;
        case cur_state is
            when S0 => 
                if i_start = '1' then
                    next_state <= S1;
                end if;    
            when S1 =>
                next_state <= S2; 
            when S2 => 
                next_state <= S3;
            when S3 => 
                 next_state <= S4; 
            when S4 => 
                if o_end = '1' then
                    next_state <= S8;
                else
                    next_state <= S44;
                end if; 
            when S44=>
                next_state <= S45;  
            when S45=>
                next_state <= S5;             
            when S5=> 
                next_state <= S6;               
            when S6 => 
                next_state <= S7;
            when S7 => 
                if o_end = '1' then
                    next_state <= S8;
                else
                    next_state <= S4;
                end if;        
            when S8 => 
                if i_start = '0' then
                    next_state <= S0;
                end if;            
        end case;
    end process;
    
    process(cur_state)
    begin
        num_sel <= '0';
        num_load <= '0';
        p1_en <= '0';
        p2_en <= '0';
        out_sel <= '0';
        out_en <= '0';
        ra_load <= '0';
        ra_sel <= '0';
        wa_load <= '0';
        wa_sel <= '0';
        rw_sel <= '0';
        last_sel <= '0';
        last_load <= '0';
        cur_load <= '0';
        o_done <= '0';
        o_en <= '0';
        o_we <= '0';
        case cur_state is
            when S0 =>
            when S1 =>
                ra_sel <= '0';
                ra_load <= '1';
                wa_sel <= '0';
                wa_load <= '1';
                last_sel <= '0';
                last_load <= '1';
            when S2 =>
                rw_sel <= '0';
                o_en <= '1';
                o_we <= '0';
                ra_load <= '1';
                ra_sel <= '1';
            when S3 =>
                num_sel <= '0';
                num_load <= '1';       
            when S4 =>
                rw_sel <= '0';
                num_sel <= '1';
                num_load <= '1';
                o_en <= '1';
                o_we <= '0';
                ra_load <= '1';
                ra_sel <= '1';
            when S44 =>
                 cur_load <= '1'; 
            when S45 =>
                p1_en <= '1';
                p2_en <= '1';         
            when S5 =>
                last_sel <= '1';
                last_load <= '1';
                out_en <= '1';
            when S6 =>
                out_sel <= '0';
                rw_sel <= '1';
                o_en <= '1';
                o_we <= '1';
                wa_load <= '1';
                wa_sel <= '1';
            when S7 =>
                out_sel <= '1';
                rw_sel <= '1';
                o_en <= '1';
                o_we <= '1';
                wa_load <= '1';
                wa_sel <= '1';
            when S8 =>
                o_done <= '1';
       end case;
   end process;        
        
        
end architecture project_reti_logiche;

----------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity datapath is
  Port ( 
          i_clk : in STD_LOGIC;
          i_rst : in STD_LOGIC;
          i_data : in std_logic_vector(7 downto 0);
          o_data : out std_logic_vector (7 downto 0);
          o_address : out std_logic_vector(15 downto 0);
          -- custom signals
          num_sel : in std_logic;
          num_load : in std_logic;
          p1_en : in std_logic;
          p2_en : in std_logic;
          out_sel : in std_logic;
          out_en : in std_logic;
          ra_load : in std_logic;
          ra_sel : in std_logic;
          wa_load : in std_logic;
          wa_sel : in std_logic;
          rw_sel : in std_logic;
          last_sel : in std_logic;
          last_load : in std_logic;
          cur_load : in std_logic;
          o_end : out std_logic
  );
end datapath;

architecture datapath of datapath is
signal mux_num : std_logic_vector (7 downto 0);
signal o_num : std_logic_vector (7 downto 0);
signal sub_num: std_logic_vector (7 downto 0);
signal o_p1 : std_logic_vector (7 downto 0);
signal o_p2 : std_logic_vector (7 downto 0);
signal o_out : std_logic_vector (15 downto 0);
signal mux_out : std_logic_vector (7 downto 0);
signal o_ra : std_logic_vector (15 downto 0);
signal mux_ra : std_logic_vector (15 downto 0);
signal sum_ra : std_logic_vector (15 downto 0);
signal o_wa : std_logic_vector (15 downto 0);
signal mux_wa : std_logic_vector (15 downto 0);
signal sum_wa : std_logic_vector (15 downto 0);
signal mux_last : std_logic_vector (1 downto 0);
signal o_last : std_logic_vector (1 downto 0);
signal o_cur : std_logic_vector (7 downto 0);

begin
    process(i_clk, i_rst) --  output num
    begin
        if(i_rst = '1') then
            o_num <= "00000000";
        elsif i_clk'event and i_clk = '1' then  
            if(num_load = '1') then
                o_num <= mux_num;
            end if;    
        end if;
    end process;
    
    process(i_clk, i_rst) --registro curInput
    begin
        if(i_rst = '1') then
            o_cur <= "00000000";
        elsif i_clk'event and i_clk = '1' then
            if(cur_load = '1') then
                o_cur <= i_data;
            end if;
        end if;
    end process;            
                     
    
    process(i_clk, i_rst) -- registro lastInput
    begin
        if(i_rst = '1') then
            o_last <= "00";
        elsif i_clk'event and i_clk = '1' then
            if(last_load = '1') then
                o_last <= mux_last;
            end if;
        end if;
    end process;
    
    with last_sel select --mux_last
        mux_last <= "00" when '0',
                     o_cur(1 downto 0) when '1',
                     "XX" when others;             
            
    
    with num_sel select -- mux_num
        mux_num <= i_data when '0',
                    sub_num when '1',
                    "XXXXXXXX" when others;
    
    sub_num <= o_num - "00000001";
    
    o_end <= '1' when (o_num = "00000000") else '0';
  
      
        process(i_clk, i_rst) --  output p1
                variable temp : std_logic_vector (7 downto 0);
                begin
                    if(i_rst = '1') then
                        o_p1 <= "00000000";
                    elsif i_clk'event and i_clk = '1' then  
                        if(p1_en = '1') then
                            temp(7) := o_cur(7) xor o_last(1);
                            temp(6) := o_cur(6) xor o_last(0);
                            for i in 5 downto 0 loop
                                temp(i) := o_cur(i) xor o_cur(i+2);
                            end loop;
                            o_p1 <= temp;    
                        end if;    
                    end if;
                end process;
               
    process(i_clk, i_rst) --  output p2
        variable temp : std_logic_vector (7 downto 0);
        begin
            if(i_rst = '1') then
                o_p2 <= "00000000";
            elsif i_clk'event and i_clk = '1' then  
                if(p2_en = '1') then
                    temp(7) := (o_cur(7) xor o_last(0)) xor o_last(1);
                    temp(6) := (o_cur(6) xor o_cur(7)) xor o_last(0);
                    for i in 5 downto 0 loop
                      temp(i) := (o_cur(i) xor o_cur(i+1)) xor o_cur(i+2);
                    end loop;
                    o_p2 <= temp;    
                end if;    
             end if;
      end process;
      
     process(i_clk, i_rst) -- generate output
     variable temp : std_logic_vector (15 downto 0);
     variable j : integer;
     begin
        if(i_rst = '1') then
            o_out <= "0000000000000000";
        elsif i_clk'event and i_clk = '1' then
            if(out_en = '1') then
                j := 15;
                for i in 7 downto 0 loop
                    temp(j) := o_p1(i);
                    temp(j-1) := o_p2(i);
                    j := j-2;
                end loop;
                o_out <= temp;
         end if;
      end if;
     end process;
     
     with out_sel select  -- divide output in high and low
        o_data <= o_out(15 downto 8) when '0',
                  o_out(7 downto 0) when '1',
                  "XXXXXXXX" when others;                 
                    
    process(i_clk, i_rst) -- registro read_address
    begin
        if(i_rst = '1')then
            o_ra <= "0000000000000000";
        elsif i_clk'event and i_clk = '1' then
            if(ra_load = '1') then
                o_ra <= mux_ra;
            end if;
        end if;
    end process;
    
    sum_ra <= o_ra + "0000000000000001";   --incremento indirizzo di lettura 
    
    with ra_sel select        -- mux read address
        mux_ra <= "0000000000000000" when '0',
                    sum_ra when '1',
                    "XXXXXXXXXXXXXXXX" when others;
    
    process(i_clk, i_rst) -- registro write address
    begin
        if(i_rst = '1') then
            o_wa <= "0000000000000000";
        elsif i_clk'event and i_clk = '1' then
            if(wa_load = '1') then
                o_wa <= mux_wa;
            end if;
        end if;
    end process;
    
    sum_wa <= o_wa + "0000000000000001";  -- incremento indirizzo di scrittura
    
    with wa_sel select                    -- mux write address
        mux_wa <= "0000001111101000" when '0',
        sum_wa when '1',
        "XXXXXXXXXXXXXXXX" when others;                                
    
    with rw_sel select
        o_address <= o_ra when '0',
                     o_wa when '1',
                     "XXXXXXXXXXXXXXXX" when others;                
                     
end architecture datapath;