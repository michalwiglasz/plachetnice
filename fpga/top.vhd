
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;



architecture main of tlv_gp_ifc is

   -- X <= (others => 'Z');

   -- pulse width modulation module
   component PWM is
      generic (
         BITS: integer
      );
      port (
         CLK, RESET: in std_logic;
         MAX_VAL: std_logic_vector(BITS-1 downto 0);
         FLIP_VAL: in std_logic_vector(BITS-1 downto 0);
         EN: in std_logic

         DOUT: out std_logic
      );
   end component;


   begin
   led_controller: PWM
      generic map (
         BITS => 25
      )
      port map (
         CLK => CLK,
         RESET => RESET,
         MAX_VAL =>  "1001100010010110100000000",
         FLIP_VAL => "0100110001001011010000000",
         DOUT => X(6)
      );


   led_controller_2: PWM
      generic map (
         BITS => 24
      )
      port map (
         CLK => CLK,
         RESET => RESET,
         MAX_VAL =>  "100110001001011010000000",
         FLIP_VAL => "010011000100101101000000",
         DOUT => X(7)
      );


   servo_controller: PWM
      generic map (
         BITS => 19
      )
      port map (
         CLK => CLK,
         RESET => RESET,
         MAX_VAL =>  "1100001101010000000",  -- 400 000 = 20 ms
         --FLIP_VAL => "0000111010100110000",  --  30 000 = 1500 us
         FLIP_VAL => "0000010111011100000",  --  600 us,
         EN => '1',
         DOUT => X(16)
      );

   X(14) <= '1';
   X(12) <= '0';


end main;

