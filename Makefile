ARCH=$(shell arch)

clean:
	rm -rf bin distrib
	rm -rf avaliador/$(ARCH) avaliador/newavaliador
	rm -rf chanomat/$(ARCH) chanomat/checkout chanomat/newchanomat
	rm -rf descr/$(ARCH) descr/newdescr
	rm -rf heuristic/$(ARCH) heuristic/heuristic
	rm -rf li/$(ARCH) li/newli
	rm -rf lib/$(ARCH) lib/libnewpoke.a
	rm -rf pokernel/$(ARCH) pokernel/newpokernel
	
