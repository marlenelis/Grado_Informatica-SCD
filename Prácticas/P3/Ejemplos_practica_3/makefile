.SUFFIXES:
.PHONY:    msg,pc,pc2,f,fc,clean
msg:
	@echo "usa 'make pc' para ejecutar prodcons."
	@echo "usa 'make pc2' para ejecutar prodcons2."
	@echo "usa 'make fs' para ejecutar filosofos."
	@echo "usa 'make fc' para ejecutar  filososfos con camarero."
	@echo "usa 'make clean' para borrar todos los archivos creados al compilar."

compilador := mpicxx
flagsc     := -g -Wall 
pc: prodcons
	mpirun -np  10 ./prodcons

pc2: prodcons2
	mpirun -np 3  ./prodcons2

fs: filosofos
	mpirun -np 10 ./filosofos

fc: filosofos_camarero
	mpirun -np 11 ./filosofos_camarero

%: %.cpp
	$(compilador) $(flagsc) -o $@ $<

clean:
	rm -f prodcons prodcons2 filosofos filosofos_camarero *~ 
