library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity servo_ctrl_reg is
   generic (
      BITS: integer
   );
   port (
      CLK, RESET: in std_logic;
      WE: in std_logic;

      DIN: in std_logic_vector(BITS-1 downto 0);
      DIN_MUX: in std_logic;

      DOUT_MAX: out std_logic_vector(BITS-1 downto 0);
      DOUT_FLIP: out std_logic_vector(BITS-1 downto 0)
   );
end servo_ctrl_reg;



architecture basic of servo_ctrl_reg is

signal max: std_logic_vector(BITS-1 downto 0);
signal flip: std_logic_vector(BITS-1 downto 0);

begin

   DOUT_MAX <= max;
   DOUT_FLIP <= flip;

   process (RESET, CLK)
   begin
     if (RESET = '1') then
         max <= (others => '0');
         flip <= (others => '0');

     elsif (CLK'event) and (CLK = '1') then
         if (WE = '1') then
            if (DIN_MUX = '0') then
               max <= DIN;
            else
               flip <= DIN;
            end If;
         end if;
     end if;
   end process;
end basic;
