# Análise orientada a objeto
> [!NOTE]
>O objetivo do sistema é o desenvolvimento de uma Estação Meteorológica capaz de realizar o monitoramento e registro de variáveis ambientais, com foco em:
    Velocidade do vento
    Direção do vento
    Índice de precipitação (pluviosidade)
    Temperatura 
> O sistema foi totalmente construído utilizando os conceitos de Programação Orientada a Objetos (POO) vistos em sala, permitindo alta reutilização de código, facilidade de manutenção e expansão para novos sensores caso seja necessário. 

## Descrição Geral do domínio do problema

>O domínio deste projeto envolve a coleta, o processamento e a apresentação visual de dados meteorológicos obtidos por sensores de hardware em campo (ou simulados em tempo real) e transmitidos via barramento USB/Serial para um dashboard de controle centralizado.
## Requisitos Funcionais

Em total conformidade com a especificação técnica do problema, o sistema atende aos seguintes requisitos estruturais:

    > Exibir Dados Meteorológicos: Apresentação em tempo real de temperatura, velocidade do vento, pluviosidade e direção analógica.

    > Filtrar Dados Meteorológicos: Mecanismo de busca e filtragem condicional na aba de histórico em horários específicos.

    > Coletar Informações dos Sensores: Escuta ativa e tratamento sintático de fluxos de dados recebidos por comunicação de hardware.

    > Verificar Status dos Sensores: Monitoramento de baixo nível para rastrear se a conexão USB com o notebook está ativa, alertando o operador visualmente sobre quedas de sinal.

## Diagrama de Casos de Uso

<p>
  <a href="img/diagrama_de_casos_de_uso_Dashoard.pdf" target="_blank">
    📄 Abrir Diagrama de Casos de Uso
  </a>
</p>

>Ator Principal: Usuário <br>

>Pré-condições
O usuário deve ter acesso ao sistema.
A estação meteorológica deve estar conectada.
Pelo menos um sensor deve estar ativo e enviando dados. <br>

>Fluxo Principal
O usuário acessa o dashboard.
O sistema estabelece comunicação com a estação meteorológica.
O sistema recebe os dados dos sensores.
O dashboard exibe os dados atualizados automaticamente.
O usuário acompanha as informações em tempo real.
O sistema continua atualizando os dados enquanto a conexão estiver ativa.


## Diagrama de Domínio do problema

<p>
  <a href="img/UML1.jpg" target="_blank">
    📄 Abrir UML
  </a>
</p>


<div align="center">

[Retroceder](README.md) | [Avançar](projeto.md)

</div>
