#!/bin/bash

EXECUTAVEL="./diogo_rocha_marques-t2-SO"
ARQUIVO_RESULTADOS="resultados.txt"
ARQUIVO_ANALISE="analise_resultados.txt"

# Limpa ou cria o arquivo de resultados
echo "Execução | Tempo Árvore (ms) | Tempo Cadeia (ms)" > "$ARQUIVO_RESULTADOS"

# Limpa ou cria o arquivo de análise
echo "Análise dos Resultados" > "$ARQUIVO_ANALISE"
echo "======================" >> "$ARQUIVO_ANALISE"

# Roda 20 vezes
for i in $(seq 1 20); do
    echo "Executando $i..."

    # Executa uma única vez e salva toda a saída
    SAIDA=$($EXECUTAVEL 3)

    # Extrai os tempos da saída
    TEMPO_ARVORE=$(echo "$SAIDA" | grep "Tempo total de criação da árvore" | awk '{print $(NF-1)}')
    TEMPO_CADEIA=$(echo "$SAIDA" | grep "Tempo total de criação da cadeia" | awk '{print $(NF-1)}')

    # Verificar se os tempos foram capturados
    if [[ -z "$TEMPO_ARVORE" || -z "$TEMPO_CADEIA" ]]; then
        echo "Erro ao obter tempos para a execução $i. Ignorando..."
        continue
    fi

    # Salvar resultado formatado
    echo "$i         | $TEMPO_ARVORE             | $TEMPO_CADEIA" >> "$ARQUIVO_RESULTADOS"
done

echo "Execuções concluídas. Resultados salvos em $ARQUIVO_RESULTADOS"

# Análise dos resultados
TEMPOS_ARVORE=$(awk 'NR>1 {print $3}' "$ARQUIVO_RESULTADOS")
TEMPOS_CADEIA=$(awk 'NR>1 {print $5}' "$ARQUIVO_RESULTADOS")

# Função para média
calcular_media() {
    local soma=0
    local count=0
    for valor in $1; do
        soma=$(echo "$soma + $valor" | bc -l)
        count=$((count + 1))
    done
    echo "$soma / $count" | bc -l
}

# Função para desvio padrão
calcular_desvio_padrao() {
    local media=$1
    local valores="$2"
    local soma_quadrados=0
    local count=0
    for valor in $valores; do
        diff=$(echo "$valor - $media" | bc -l)
        quad=$(echo "$diff * $diff" | bc -l)
        soma_quadrados=$(echo "$soma_quadrados + $quad" | bc -l)
        count=$((count + 1))
    done
    echo "scale=3; sqrt($soma_quadrados / $count)" | bc -l
}

MEDIA_ARVORE=$(calcular_media "$TEMPOS_ARVORE")
MEDIA_CADEIA=$(calcular_media "$TEMPOS_CADEIA")

DESVIO_ARVORE=$(calcular_desvio_padrao "$MEDIA_ARVORE" "$TEMPOS_ARVORE")
DESVIO_CADEIA=$(calcular_desvio_padrao "$MEDIA_CADEIA" "$TEMPOS_CADEIA")

COUNT=$(echo "$TEMPOS_ARVORE" | wc -l)

# Escrever a análise
{
    echo "Média dos Tempos Árvore: $MEDIA_ARVORE ms"
    echo "Desvio Padrão Árvore: $DESVIO_ARVORE ms"
    echo "Média dos Tempos Cadeia: $MEDIA_CADEIA ms"
    echo "Desvio Padrão Cadeia: $DESVIO_CADEIA ms"
    echo "Número de Execuções: $COUNT"
    echo "Análise concluída. Resultados salvos em $ARQUIVO_ANALISE"
} >> "$ARQUIVO_ANALISE"
