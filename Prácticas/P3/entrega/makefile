.SUFFIXES:
.PHONY:    msg,pc,fs,fc,clean
msg:
	@echo "usa 'make pc' para ejecutar prodcons."
	@echo "usa 'make pm' para ejecutar prodcons_mod."
	@echo "usa 'make fs' para ejecutar filosofos."
	@echo "usa 'make fc' para ejecutar  filososfos con camarero."
	@echo "usa 'make clean' para borrar todos los archivos creados al compilar."

compilador := mpicxx
flagsc     := -g -Wall 
BIN	=	bin

pc: $(BIN)/prodcons
	mpirun -np  10 ./$(BIN)/prodcons

pm: $(BIN)/prodcons_mod
	mpirun -np 5 ./$(BIN)/prodcons_mod
	
fs: $(BIN)/filosofos
	mpirun -np 10 ./$(BIN)/filosofos

fc:$(BIN)/filosofos_camarero
	mpirun -np 11 ./$(BIN)/filosofos_camarero

$(BIN)/%: %.cpp
	$(compilador) $(flagsc) -o $@ $<

clean:
	rm -f  $(BIN)/* $(BIN)/*~  *~ 
