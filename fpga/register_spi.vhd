library ieee;
use ieee.std_logic_1164.all;


entity register_spi is
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
end register_spi;



architecture basic of register_spi is

-- spi decoder module
component SPI_adc
   generic (
      ADDR_WIDTH : integer;
      DATA_WIDTH : integer;
      ADDR_OUT_WIDTH : integer;
      BASE_ADDR : integer
   );
   port (
      CLK      : in  std_logic;
      CS       : in  std_logic;
      DO       : in  std_logic;
      DO_VLD   : in  std_logic;
      DI       : out std_logic;
      DI_REQ   : in  std_logic;
      ADDR     : out  std_logic_vector (ADDR_OUT_WIDTH-1 downto 0);
      DATA_OUT : out  std_logic_vector (DATA_WIDTH-1 downto 0);
      DATA_IN  : in   std_logic_vector (DATA_WIDTH-1 downto 0);
      WRITE_EN : out  std_logic;
      READ_EN  : out  std_logic
   );
end component;

-- servo control register module
component reg is
   generic (
      BITS: integer
   );
   port (
      CLK, RESET: in std_logic;
      WE: in std_logic;
      DIN: in std_logic_vector(BITS-1 downto 0);
      DOUT: out std_logic_vector(BITS-1 downto 0)
   );
end component;



signal spi_addr: std_logic_vector(0 downto 0);
signal spi_din: std_logic_vector(15 downto 0);
signal spi_dout: std_logic_vector(15 downto 0);
signal spi_we : std_logic;


begin
  DOUT <= spi_din;

  spi: SPI_adc
     generic map (
       ADDR_WIDTH => 8,
       DATA_WIDTH => BITS,
       ADDR_OUT_WIDTH => 1,
       BASE_ADDR  => BASE_ADDR
    )
    port map (
       CLK      => CLK,
       CS       => SPI_CS,
       DO       => SPI_DO,
       DO_VLD   => SPI_DO_VLD,
       DI       => SPI_DI,
       DI_REQ   => SPI_DI_REQ,

       ADDR     => spi_addr,
       DATA_OUT => spi_dout,
       DATA_IN  => spi_din,
       WRITE_EN => spi_we,
       READ_EN  => open
    );


  data: reg
     generic map (
        BITS => BITS
     )
     port map (
        CLK => CLK,
        RESET => RESET,
        WE => spi_we,
        DIN => spi_dout,
        DOUT => spi_din
     );

end basic;