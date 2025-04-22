

### AVALIAÇÃO DE DESEMPENHO

Foi utilizado o programa `diogo_rocha_marques-t2-SO.c` para gerar as árvores e pegar os PIDs e tempos de processos. Para realizar os testes de maneira sucinta, foi utilizado o código `executar_20_vezes.sh`.

#### Código Bash (executar_20_vezes.sh):
```bash
#!/bin/bash

# Nome do arquivo executável
EXECUTAVEL="./main"

# Arquivo de saída para salvar os resultados
ARQUIVO_RESULTADOS="resultados.txt"
ARQUIVO_ANALISE="analise_resultados.txt"

# Limpa ou cria o arquivo de resultados
echo "Execução | Tempo Árvore (ms)" > $ARQUIVO_RESULTADOS

# Limpa ou cria o arquivo de análise
echo "Análise dos Resultados" > $ARQUIVO_ANALISE
echo "======================" >> $ARQUIVO_ANALISE

# Roda 20 vezes
for i in $(seq 1 20); do
    echo "Executando $i..."
    # Roda o programa e pega o tempo
    TEMPO=$(./main 3 | grep "Tempo total de criação da árvore" | awk '{print $7}')
    
    # Verificar se o tempo não está vazio
    if [[ -z "$TEMPO" ]]; then
        echo "Erro ao obter tempo para a execução $i. Ignorando..."
        continue
    fi
    
    # Salva os resultados
    echo "$i        | $TEMPO" >> $ARQUIVO_RESULTADOS
done

echo "Execuções concluídas. Resultados salvos em $ARQUIVO_RESULTADOS"

# Análise dos resultados

# Extrair os tempos de execução para análise
TEMPOS=$(awk '{print $3}' $ARQUIVO_RESULTADOS | tail -n +2)

# Calcular a média
SOMA=0
COUNT=0
for TEMPO in $TEMPOS; do
    SOMA=$(echo "$SOMA + $TEMPO" | bc)
    COUNT=$((COUNT + 1))
done

# Verificar se COUNT é maior que zero
if [[ $COUNT -eq 0 ]]; then
    echo "Nenhum tempo válido foi coletado. Abortando análise." >> $ARQUIVO_ANALISE
    exit 1
fi

MEDIA=$(echo "$SOMA / $COUNT" | bc -l)

# Calcular o desvio padrão
SOMA_QUADRADOS=0
for TEMPO in $TEMPOS; do
    DIFERENCA=$(echo "$TEMPO - $MEDIA" | bc -l)
    QUADRADO=$(echo "$DIFERENCA * $DIFERENCA" | bc -l)
    SOMA_QUADRADOS=$(echo "$SOMA_QUADRADOS + $QUADRADO" | bc -l)
done

DESVIO_PADRAO=$(echo "scale=3; sqrt($SOMA_QUADRADOS / $COUNT)" | bc -l)

# Escrever a análise no arquivo
echo "Média dos Tempos: $MEDIA ms" >> $ARQUIVO_ANALISE
echo "Desvio Padrão: $DESVIO_PADRAO ms" >> $ARQUIVO_ANALISE
echo "Número de Execuções: $COUNT" >> $ARQUIVO_ANALISE

echo "Análise concluída. Resultados salvos em $ARQUIVO_ANALISE"
```

#### Análise dos Resultados:

- **Média dos Tempos**: A média do tempo de execução foi de **6.85 ms**.
  - Isso significa que, em média, o tempo necessário para a criação da árvore binária de processos foi de **6.85 milissegundos** por execução.

- **Desvio Padrão**: O desvio padrão foi de **0.97 ms**.
  - O desvio padrão mede a dispersão dos tempos em relação à média. Com um valor de **0.97 ms**, indica que a maioria dos tempos de execução está muito próxima da média, com variações relativamente pequenas.

- **Distribuição dos Tempos**: Os tempos de cada execução foram os seguintes:

```text
Execução  | Tempo Árvore (ms)
1         | 5.195
2         | 4.628
3         | 6.560
4         | 7.070
5         | 6.435
6         | 5.517
7         | 7.420
8         | 6.362
9         | 7.179
10        | 8.262
11        | 6.427
12        | 6.176
13        | 6.851
14        | 7.691
15        | 7.613
16        | 7.710
17        | 8.058
18        | 7.370
19        | 6.383
20        | 8.130
```

- **Tendências**:
  - A maioria das execuções teve tempos entre **4.6 ms** e **8.1 ms**, o que está muito próximo da média (**6.85 ms**).
  - As execuções **2** e **19** apresentaram os tempos mais baixos e mais altos, respectivamente, mas dentro de um intervalo esperado.
  - O tempo mais alto foi **8.262 ms** na execução 10 e o mais baixo foi **4.628 ms** na execução 2. Esses valores estão relativamente próximos, o que reforça a estabilidade observada no desvio padrão.

#### Conclusões:
- **Estabilidade**: O sistema apresenta boa estabilidade no tempo de execução, com um desvio padrão de apenas **0.97 ms**, o que indica pouca variação entre as execuções.
- **Eficiência**: A média de **6.85 ms** é razoável para o processo de criação de múltiplos processos, estando dentro de um intervalo previsível.
- **Execuções Extremas**: Embora o tempo entre a execução mais baixa e a mais alta seja de cerca de **3.6 ms**, isso é esperado em sistemas com múltiplos processos devido à sobrecarga da criação de processos, que pode variar com o agendamento do sistema operacional.

---

### Resumo dos Resultados Gerados

Após a execução do código **`executar_20_vezes.sh`**, foram gerados dois arquivos de saída:

1. **`resultados.txt`**: Contém os tempos de execução de cada uma das 20 execuções, conforme mostrado na tabela acima.
2. **`analise_resultados.txt`**: Contém o resumo dos resultados, que inclui os seguintes dados:

```text
Análise dos Resultados
======================
Média dos Tempos: 6.85185000000000000000 ms
Desvio Padrão: .965 ms
Número de Execuções: 20
```

Esses arquivos de saída permitem uma análise detalhada do desempenho do programa de criação de árvores binárias, com ênfase na estabilidade e eficiência do sistema.

