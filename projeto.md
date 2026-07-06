# Projeto orientado a objeto

>[!NOTE]
>Descrição da Classe: sensorPluviometro

A classe sensorPluviometro é uma especialização direta da classe base Sensor. Ela foi projetada sob o conceito de herança para encapsular o comportamento e o tratamento de dados específicos do sensor de precipitação (pluviômetro) da estação meteorológica.
>Atributos (Variáveis Privadas)
    precipitacaoAtual (double / int): Armazena o volume de chuva acumulado lido pelo sensor em um determinado intervalo de tempo.
    unidadePrecipitado (string): String constante que define a unidade de medida do sensor físico, configurada padronizadamente como "mm" (milímetros).
    labelChuva (QLabel*): Ponteiro para o componente gráfico do framework Qt encarregado de renderizar o valor numérico tratado diretamente na interface do usuário.

>Métodos (Funções Membro)
    medirPrecipitacao() / setChuva(double chuva): Recebe o pulso elétrico ou sinal bruto enviado pelo hardware do pluviômetro de báscula (ex: modelo PB10), converte o dado para a escala em milímetros, atualiza o atributo interno precipitacaoAtual e atualiza o texto da labelChuva na tela em tempo real.

>Relações no Diagrama de Classes
    Herança (Sensor <|-- SensorChuva): Ela herda as propriedades genéricas da classe mãe Sensor (como identificadores de fábrica, strings de metadados e rotinas de checagem de erros), estendendo-as para gerenciar especificamente os índices de chuva.



<div align="center">

[Retroceder](analise.md) | [Avançar](implementacao.md)

</div>
