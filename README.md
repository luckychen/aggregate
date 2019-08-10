## The aggregate code for sequence data

This is the primary version of aggregate project

To run this code please build with

```
make

```
A executable file <b>test</b> will be created to run the aggregation, 


```
./test -i [input file name] -o [output file name]

```

For example, we have a input file named <b>all_sample.reads</b> 
and we want to generate a file named <b>all_samples_result</b>

please run

```
./test -i all_sample.reads -o all_samples_result
```
The results will put the "groupID" on the first column and the original data from the input file to the following columns

for example, 

```
groupID	readSeq	readLength	readId	readFreq	readMax	Ron030819-DNA-GFP-Positive_S3_format	Ron030819-DNA-GFP-Negative_S8_format	Ron030819-RNA-GFP-Positive_S7_format	Ron030819-RNA-GFP-Negative_S6_format	Ron030819-blockDNA-GFP-Positive_S4_format	Ron030819-blockDNA-GFP-Negative_S1_format	Ron030819-blockRNA-GFP-Positive_S5_format	Ron030819-blockRNA-GFP-Negative_S2_format
0	CCAGTGGTCAAGTGCTTTCAATTTTCCCTCCT	32	1283	8	169	169	25	21	1	96	21	21	1
0	CCACTTGTCAAGTGCTTTCAATTTTCCCTCCT	32	677	8	507	507	101	51	1	272	62	47	3
0	CCACTGGTTAAGTGCTTTCAATTTTCCCTCCT	32	708	8	456	456	104	65	1	372	78	80	1
0	CCACTGGTCATGTGCTTTCAATTTTCCCTCCT	32	743	8	419	419	79	37	2	240	47	47	2
0	CCACTGGTCAATTGCTTTCAATTTTCCCTCCT	32	589	8	646	646	107	64	4	325	79	52	1
0	CCACTGGTCAAGTTCTTTCAATTTTCCCTCCT	32	499	8	898	898	204	71	1	476	135	79	2
0	CCACTGGTCAAGTGCTTTCTATTTTCCCTCCT	32	524	8	821	821	141	86	1	426	102	86	1
0	CCACTGGTCAAGTGCTTTCGATTTTCCCTCCT	32	741	8	421	421	63	36	1	233	48	33	2
0	CCACTGGTCAAGTGCTTTCATTTTTCCCTCCT	32	707	8	456	456	68	40	1	266	38	35	1
0	CCACTGGTCAAGTGCTTTCAATTTTCCCTTCT	32	745	8	417	417	74	39	1	293	62	56	4
0	CCACTGGTCAAGTGCTTTCAATTTTCCCTCGT	32	787	8	368	368	62	52	1	230	51	39	1
0	CCACTGGTCAAGTGCTTTCAATTTTCCCTCAT	32	593	8	638	638	109	66	2	364	74	77	2
0	CCACTGGTCAAGTGCTTTCAATTTTCCCTACT	32	647	8	549	549	87	46	5	322	79	53	1
0	CCACTGGTCAAGTGCTTTCAATTTTCCCACCT	32	878	8	303	303	43	32	1	171	43	32	2
0	CCACTGGTCAAGTGCTTTCAATTTCCCCTCCT	32	669	8	516	516	65	43	2	293	56	45	2
0	CCACTGGTCAACTGCTTTCAATTTTCCCTCCT	32	1300	8	164	164	36	18	1	97	21	16	1
0	CCACTAGTCAAGTGCTTTCAATTTTCCCTCCT	32	620	8	585	585	122	62	1	357	85	73	1
0	CCACAGGTCAAGTGCTTTCAATTTTCCCTCCT	32	458	8	1099	1099	166	103	2	561	131	114	2
0	CCAATGGTCAAGTGCTTTCAATTTTCCCTCCT	32	471	8	1029	1029	180	108	4	574	145	107	1
0	CCACTGTTCAAGTGCTTTCAATTTTCCCTCCT	32	838	7	336	336	63	23	1	168	37	30	
0	CCACTGGTCAAGTGCTTTCACTTTTCCCTCCT	32	2058	7	77	77	13	10	1	39	10	6	
0	CCACTGGTCAAGTGCTTTCAATTTTCCCTCTT	32	857	7	321	321	76	40		253	73	62	2
0	CCACTGGTCAAGTGCTTTCAATTTACCCTCCT	32	833	7	340	340	53	29	1	185	40	39	

```
The lines with same "groupID" means they blongs to same group. A Seq belongs to a group if it hase equal or less 1-charator distance to any other Seqs in that group

Please test this code and report bugs to me. Thanks!
