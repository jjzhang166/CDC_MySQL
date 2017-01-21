
MAKE := make --no-print-directory

all :
	@$(MAKE) -f makefile_CDC_MySQL clean;
	@$(MAKE) -f makefile_CDC_MySQL;
	
	@$(MAKE) -f makefile_Tests clean;
	@$(MAKE) -f makefile_Tests;

clean :
	@$(MAKE) -f makefile_CDC_MySQL clean;
	@$(MAKE) -f makefile_Tests clean;
	


