.PHONY: clean All

All:
	@echo ----------Building project:[ sebgpd - Debug ]----------
	@cd "sebgpd" && "$(MAKE)" -f "sebgpd.mk"
clean:
	@echo ----------Cleaning project:[ sebgpd - Debug ]----------
	@cd "sebgpd" && "$(MAKE)" -f "sebgpd.mk" clean
