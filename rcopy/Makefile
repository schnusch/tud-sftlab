all: c cpp java rdiff
	$(MAKE) -C c $@
	$(MAKE) -C cpp $@
	cd java && ant build
	$(MAKE) -C rdiff $@

clean: c cpp java rdiff
	$(MAKE) -C c $@
	$(MAKE) -C cpp $@
	cd java && ant $@
	$(MAKE) -C rdiff $@
	$(RM) gpl.txt gpl[23].txt lpg.txt lpg[23].txt \
			c-gpl.txt c-gpl[23].txt c-lpg.txt c-lpg[23].txt \
			j-gpl.txt j-gpl[23].txt j-lpg.txt j-lpg[23].txt

tests: test-rdiff test-rcopy test-RCopy

test-files-rdiff: gpl.txt lpg.txt gpl2.txt lpg2.txt gpl3.txt lpg3.txt
test-rdiff: rdiff/rdiff test-files-rdiff
	@for g in gpl.txt gpl[23].txt; do \
		for l in lpg.txt lpg[23].txt; do \
			echo rdiff/rdiff $$g $$l; rdiff/rdiff $$g $$l; echo error: $$?; \
			echo rdiff/rdiff $$l $$g; rdiff/rdiff $$l $$g; echo error: $$?; \
			echo; \
		done; \
	done

test-files-rcopy: c-gpl.txt c-lpg.txt
test-rcopy: c/rcopy test-files-rcopy test-files-rdiff
	diff -u lpg.txt c-gpl.txt
	diff -u gpl.txt c-lpg.txt

test-files-RCopy: j-gpl.txt j-lpg.txt
test-RCopy: java/build/RCopy.class test-files-RCopy test-files-rdiff
	diff -u lpg.txt j-gpl.txt
	diff -u gpl.txt j-lpg.txt

c/rcopy:
	$(MAKE) -C c rcopy
java/build/RCopy.class:
	cd java && ant build
rdiff/rdiff:
	$(MAKE) -C rdiff rdiff

c-%: % c/rcopy
	c/rcopy $< $@
j-%: % java/build/RCopy.class
	java -cp java/build RCopy $< $@

gpl.txt:
	curl -o $@ http://www.gnu.org/licenses/gpl-2.0.txt
lpg.txt: gpl.txt
	{ echo; tac $< | rev | head -c-1; } > $@

gpl2.txt: gpl.txt
	head -c-1 $< > $@
lpg2.txt: lpg.txt
	tail -c+2 $< > $@

gpl3.txt: gpl.txt
	head -c-2 $< > $@
lpg3.txt: lpg.txt
	tail -c+3 $< > $@
