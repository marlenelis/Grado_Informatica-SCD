
.SUFFIXES:
.PHONY: msg_uso msg_clases

JAVASRCS:=$(wildcard *.java)
CLASSES_BASE:=$(shell grep class *.java | cut -d' ' -f2)
CLASSES:=$(addsuffix .class,$(CLASSES_BASE))

msg_uso:
	@echo "SCD: Ejemplos de monitores en Java."
	@echo "usar con:"
	
	@echo "   make pc    : compilar todo y ejecutar productor-consumidor "
	@echo "   make fm    : idem para fumadores"
	@echo "   make bd   : idem para barberoDurmiente"
	@echo "   make bm   : idem para barberoDurmiente_m"
	@echo "   make clean : elimina todos los archivos .class"
	
comp: msg_clases monitor_classes $(CLASSES)

recomp:
	make clean	
	make comp

monitor_classes: $(wildcard monitor/*.class)
	javac -g monitor/*.java -d .

clean:
	rm -rf *.class monitor/*.class 
	
pc: prodcons.class
	java prodcons
	
fm: fumadores.class
	java fumadores

bd: barberoDurmiente.class
	java barberoDurmiente

bm: barberoDurmiente_m.class
	java barberoDurmiente_m
	
msg_clases:
	@echo "fuentes java = " $(JAVASRCS)
	@echo "clases = " $(CLASSES_BASE)
	


