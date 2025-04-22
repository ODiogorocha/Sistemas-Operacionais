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
