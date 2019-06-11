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
1	TTGACAACTCTTTGTGTTTCATCCAATCCCG	31	3	8	645393	66604	95	632670	438	39116	9860	645393	629
1	TTGACAACTCTTTGTGTTTCATCCAATCCCA	31	551	5	758	36		683		23	14	758	
1	TTGACAACTCTTTGTGTTTCATCCAATCCCT	31	592	7	644	79		644	2	44	19	604	1
1	TTGACAACTCTTTGTGTTTCATCCAATCCCC	31	1745	5	101	16		96		10	2	101	
1	TTGACAACTCTTTGTGTTTCATCCAATCCCN	31	43151	2	2			2				1	
1	TTGACAACTCTTTGTGTTTCNTCCAATCCCG	31	30378	1	3							3	
2	CCACTGGTCAAGTGCTTTCAATTTTCCCTCCT	32	1	8	1849979	1849979	300649	159103	3443	1029115	223853	183935	4620
2	CCACTGGTCAAGTGCTTTCAATTTTCCCTCCA	32	831	8	341	341	41	40	2	194	45	41	1
2	CCACTGGTCAAGTGCTTTCAATTTTCCCTCCC	32	889	8	296	296	50	22	1	135	45	24	1
2	CCACTGGTCAAGTGCTTTCAATTTTCCCTCCG	32	1256	7	173	173	24	26		88	23	23	1
2	CCACTGGTCAAGTGCTTTCAATTTTCCCTCCN	32	217624	1	1	1							
2	CCACTGGTCAAGTGCTTTCNATTTTCCCTCCT	32	63035	2	1	1						1	
2	CCNCTGGTCAAGTGCTTTCAATTTTCCCTCCT	32	8778	4	10	10	3			3		2	
2	NCNCTGGTCAAGTGCTTTCAATTTTCCCTCCT	32	16557	3	5	2				5	1		
2	NNNCTGGTCAAGTGCTTTCAATTTTCCCTCCT	32	363931	1	1	1							
3	TTTAGTCCTTACAAAGCTACCGTCATTTGGTC	32	5	8	509676	376	509676	105	58545	9990	327950	2474	72845
3	TTTAGTCCTTACAAAGCTACCGTCATTTGGTT	32	872	6	308		308		28	6	177	1	55
3	TTTAGTCCTTACAAAGCTACCGTCATTTGGTA	32	1455	6	136		136		11	6	85	2	18
3	TTTAGTCCTTACAAAGCTACCGTCATTTGGTG	32	1531	6	126		126		16	1	82	1	23
3	TTTAGTCCTTACAAAGCTACCGTCNTTTGGTC	32	28970	2	3		3						2
3	TTTAGTCCTTACAAAGCTNCCGTCATTTGGTC	32	471327	1	1		1						
3	TTTCGTCCTTACAAAGCTACCGTCATTTGGTC	32	8354	3	11		11				6		1
4	TGTCCCCGTGCTTAGATAATCTAAAGTTAGTG	32	7	8	336482	234	336482	520	89029	6687	219563	2108	111851
4	TGTCCCCGTGCTTAGATAATCTAAAGTTAGTT	32	1016	6	240		240		61	5	137	3	74
4	TGTCCCCGTGCTTAGATAATCTAAAGTTAGTA	32	1760	7	99		99	1	23	2	88	1	39
4	TGTCCCCGTGCTTAGATAATCTAAAGTTAGTC	32	2540	5	55		55		12	1	31		8
4	TGTCCCCGTGCTTAGATAATCTAAAGTTNGTG	32	441752	1	1								1
4	TGTCCCCGTGCTTAGNTAATCTAAAGTTAGTG	32	442180	1	1						1		
4
```
The lines with same "groupID" means they blongs to same group. A Seq belongs to a group if it hase equal or less 1-charator distance to any other Seqs in that group

Please test this code and report bugs to me. Thanks!
