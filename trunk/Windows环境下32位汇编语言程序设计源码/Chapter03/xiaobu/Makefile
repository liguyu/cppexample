NAME = xiaobu
OBJS = $(NAME).obj

LINK_FLAG = /subsystem:windows /DEBUG /DEBUGTYPE:CV
ML_FLAG = /c /coff /Cp /Zi

$(NAME).exe: $(OBJS)
	Link $(LINK_FLAG) $(OBJS)
.asm.obj:
	ml $(ML_FLAG) $<

clean:
	del *.obj
