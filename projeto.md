# 📐 Projeto Orientado a Objeto

> [!NOTE]
> ### Descrição da Classe: `sensorpluviometro`

A classe `sensorpluviometro` é uma especialização direta da classe base `Sensor`. Ela foi projetada sob o conceito de herança para encapsular o comportamento e o tratamento de dados específicos do sensor de precipitação (pluviômetro) da estação meteorológica.

### Atributos (Variáveis Privadas)
*   **`precipitacaoAtual` (`double`):** Armazena o volume de chuva acumulado lido pelo sensor em um determinado intervalo de tempo.
*   **`unidadePrecipitacao` (`string`):** String constante que define a unidade de medida do sensor físico, configurada padronizadamente como `"mm"` (milímetros).
*   **`labelChuva` (`QLabel*`):** Ponteiro para o componente gráfico do framework Qt encarregado de renderizar o valor numérico tratado diretamente na interface do usuário.

### Métodos (Funções Membro)
*   **`setChuva(double chuva)`:** Recebe o sinal bruto enviado pelo hardware do pluviômetro de báscula digital (como o modelo PB10), converte o dado para a escala em milímetros, atualiza o atributo interno e modifica o texto da `labelChuva` na interface gráfica em tempo real.

### Relações no Diagrama de Classes
*   **Herança (`Sensor <|-- sensorpluviometro`):** Ela herda as propriedades e assinaturas genéricas da classe mãe `Sensor`, estendendo-as para gerenciar especificamente os índices de pluviosidade da estação.

---

<div align="center">

[⬅️ Retroceder](analise.md) | [Avançar ➡️](implementacao.md)

</div>
