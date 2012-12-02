
library ieee;
use ieee.std_logic_1164.all;



architecture main of tlv_gp_ifc is

   component register_spi is
      generic (
         BITS         : integer;
         BASE_ADDR    : integer
      );
      port (
         CLK          : in  std_logic;
         RESET        : in  std_logic;
         EN           : in  std_logic;

         SPI_CS       : in  std_logic;
         SPI_DO       : in  std_logic;
         SPI_DO_VLD   : in  std_logic;
         SPI_DI       : out std_logic;
         SPI_DI_REQ   : in  std_logic;

         DOUT         : out std_logic_vector(BITS-1 downto 0)
      );
   end component;


   component servo
      generic (
         BASE_ADDR: integer
      );
      port (
         CLK          : in  std_logic;
         RESET        : in  std_logic;
         EN           : in  std_logic;

         SPI_CS       : in  std_logic;
         SPI_DO       : in  std_logic;
         SPI_DO_VLD   : in  std_logic;
         SPI_DI       : out std_logic;
         SPI_DI_REQ   : in  std_logic;

         SERVO        : out std_logic;
         LED          : out std_logic
      );
   end component;

   signal ctrl_reg : std_logic_vector(15 downto 0);

begin

    X(45 downto 30) <= ctrl_reg(15 downto 0);

   ctrl: register_spi
     generic map (
        BITS         => 16,
        BASE_ADDR    => 16#0060#
     )
     port map (
        CLK          => CLK,
        RESET        => RESET,
        EN           => '0',

        SPI_CS       => SPI_CS,
        SPI_DO       => SPI_DO,
        SPI_DO_VLD   => SPI_DO_VLD,
        SPI_DI       => SPI_DI,
        SPI_DI_REQ   => SPI_DI_REQ,

        DOUT         => ctrl_reg
     );


   mast: servo
     generic map (
        BASE_ADDR    => 16#00A0#
     )
     port map (
        CLK          => CLK,
        RESET        => RESET,
        EN           => ctrl_reg(15),

        SPI_CS       => SPI_CS,
        SPI_DO       => SPI_DO,
        SPI_DO_VLD   => SPI_DO_VLD,
        SPI_DI       => SPI_DI,
        SPI_DI_REQ   => SPI_DI_REQ,

        SERVO        => X(18),
        LED          => X(21)
     );


  wheels: servo
     generic map (
        BASE_ADDR    => 16#00B0#
     )
     port map (
        CLK          => CLK,
        RESET        => RESET,
        EN           => ctrl_reg(14),

        SPI_CS       => SPI_CS,
        SPI_DO       => SPI_DO,
        SPI_DO_VLD   => SPI_DO_VLD,
        SPI_DI       => SPI_DI,
        SPI_DI_REQ   => SPI_DI_REQ,

        SERVO        => X(16),
        LED          => X(20)
     );

end main;
