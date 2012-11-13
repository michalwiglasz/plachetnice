library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity PWM is
   generic (
      BITS: integer
   );
   port (
      CLK, RESET: in std_logic;
      MAX_VAL: in std_logic_vector(BITS-1 downto 0);
      FLIP_VAL: in std_logic_vector(BITS-1 downto 0);
      EN: in std_logic;

      DOUT: out std_logic
   );
end PWM;


architecture basic of PWM is
signal cnt : std_logic_vector(BITS-1 downto 0) := (others => '0');
signal rst: std_logic;
signal flip: std_logic;

begin
   flip <= '1' when (cnt=FLIP_VAL) else '0';
   rst <= '1' when (cnt=MAX_VAL) else '0';

   process(RESET, CLK)
   begin
      if (EN = '1') then
         if (RESET = '1') then
            cnt <= (others => '0');
            DOUT <= '1';

         elsif CLK'event and CLK = '1' then
            if (rst='1') then
               cnt <= (others => '0');
               DOUT <= '1';

            else
               cnt <= cnt + 1;
               if (flip = '1') then
                  DOUT <= '0';
               end If;
            end If;
         end If;

      else
         -- if EN = 0, disable output
         DOUT <= '0';
      end If;
   end process;
end basic;
