
i) Estrutura das diretorias

	ProjSO_TiagoNeves_PedroLeitao.zip

		- Pasta "CircuitRouter-ParSolver"  (versao 1 - Uso o trinco global)
			-> CircuitRouter-ParSolver.c e CircuitRouter-ParSolver.o
			-> coordinate.c , coordinate.h e coordinate.o
			-> grid.c , grid.h e grid.o
			-> MakeFile
			-> maze.c , maze.h e maze.o 
			-> router.c , router.h , router.o

		- Pasta "CircuitRouter-ParSolver v2"  (versao 2 - Uso granulidade fina)
			-> CircuitRouter-ParSolver.c e CircuitRouter-ParSolver.o
			-> coordinate.c , coordinate.h e coordinate.o
			-> grid.c , grid.h e grid.o
			-> MakeFile
			-> maze.c , maze.h e maze.o 
			-> router.c , router.h , router.o

		- README.txt

		- doTest.sh

		- Pasta "results" (onde estao os ficheiros do speed-up ate 256x256 com a versao 1 (Trinco global))


ii) Os passos para compilar e executar o projeto

	(..)/CircuitRouter-ParSolver$ make

	(..)/CircuitRouter-ParSolver$ ./CircuitRouter-ParSolver -t [NUMTAREFAS] [inputfile]

	exemplo: 					  ./CircuitRouter-ParSolver -t 3 ../inputs/random-x32-y32-z3-n64.txt


	Executar doTest.sh

	(..)/doTest.sh [NUMTAREFAS] [inputfile]



iii) Descricao das carateristicas do processador (tenho o numero de processadores que e 8)
(ultimo processor que e 7)

	processor	: 7
	vendor_id	: GenuineIntel
	cpu family	: 6
	model		: 142
	model name	: Intel(R) Core(TM) i7-8550U CPU @ 1.80GHz
	stepping	: 10
	microcode	: 0x96
	cpu MHz		: 1000.061
	cache size	: 8192 KB
	physical id	: 0
	siblings	: 8
	core id		: 3
	cpu cores	: 4
	apicid		: 7
	initial apicid	: 7
	fpu		: yes
	fpu_exception	: yes
	cpuid level	: 22
	wp		: yes
	flags		: fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp lm constant_tsc art arch_perfmon pebs bts rep_good nopl xtopology nonstop_tsc cpuid aperfmperf tsc_known_freq pni pclmulqdq dtes64 monitor ds_cpl vmx est tm2 ssse3 sdbg fma cx16 xtpr pdcm pcid sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand lahf_lm abm 3dnowprefetch cpuid_fault epb invpcid_single pti ssbd ibrs ibpb stibp tpr_shadow vnmi flexpriority ept vpid fsgsbase tsc_adjust bmi1 avx2 smep bmi2 erms invpcid mpx rdseed adx smap clflushopt intel_pt xsaveopt xsavec xgetbv1 xsaves dtherm ida arat pln pts hwp hwp_notify hwp_act_window hwp_epp flush_l1d
	bugs		: cpu_meltdown spectre_v1 spectre_v2 spec_store_bypass l1tf
	bogomips	: 3984.00
	clflush size	: 64
	cache_alignment	: 64
	address sizes	: 39 bits physical, 48 bits virtual
	power management:




iii) Sistema Operativo

	Linux tiago 4.15.0-36-generic #39~16.04.1-Ubuntu SMP Tue Sep 25 08:59:23 UTC 2018 x86_64 x86_64 x86_64 GNU/Linux
