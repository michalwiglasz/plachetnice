library ieee;
use ieee.std_logic_1164.all;

entity reg is
   generic (
      BITS: integer
   );
   port (
      CLK, RESET: in std_logic;
      WE: in std_logic;

      DIN: in std_logic_vector(BITS-1 downto 0);
      DOUT: out std_logic_vector(BITS-1 downto 0)
   );
end reg;



architecture basic of reg is

signal value: std_logic_vector(BITS-1 downto 0);

begin
   DOUT <= value;

   process (RESET, CLK)
   begin
     if (RESET = '1') then
         value <= (others => '0');

     elsif (CLK'event) and (CLK = '1') then
         if (WE = '1') then
            value <= DIN;
         end if;
     end if;
   end process;
end basic;
