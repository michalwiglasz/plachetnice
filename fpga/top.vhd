
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;



architecture main of tlv_gp_ifc is

   component servo
      generic (
         BASE_ADDR: integer
      );
      port (
         CLK          : in  std_logic;
         RESET        : in  std_logic;

         SPI_CS       : in  std_logic;
         SPI_DO       : in  std_logic;
         SPI_DO_VLD   : in  std_logic;
         SPI_DI       : out std_logic;
         SPI_DI_REQ   : in  std_logic;

         SERVO        : out std_logic;
         LED          : out std_logic
      );
   end component;

begin

   mast: servo
     generic map (
        BASE_ADDR    => 16#00A0#
     )
     port map (
        CLK          => CLK,
        RESET        => RESET,

        SPI_CS       => SPI_CS,
        SPI_DO       => SPI_DO,
        SPI_DO_VLD   => SPI_DO_VLD,
        SPI_DI       => SPI_DI,
        SPI_DI_REQ   => SPI_DI_REQ,

        SERVO        => X(18),
        LED          => LEDF
     );


  wheels: servo
     generic map (
        BASE_ADDR    => 16#00B0#
     )
     port map (
        CLK          => CLK,
        RESET        => RESET,

        SPI_CS       => SPI_CS,
        SPI_DO       => SPI_DO,
        SPI_DO_VLD   => SPI_DO_VLD,
        SPI_DI       => SPI_DI,
        SPI_DI_REQ   => SPI_DI_REQ,

        SERVO        => X(16),
        LED          => X(20)
     );

end main;
