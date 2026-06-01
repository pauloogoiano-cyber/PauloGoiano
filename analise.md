# Análise orientada a objeto
> [!NOTE]
> O objetivo do sistema é desenvolver uma Estação Meteorológica capaz de monitorar e registrar variáveis ambientais, com o foco em: Velocidade, Direção do vento e Índice de precipitação.
> O sistema deve ser estruturado utilizando os conceitos de POO visto em sala, permitindo a reutilização do código, facilidade de manutenção e expansão para novos sensores caso venha a ser preciso. 

## Descrição Geral do domínio do problema

> O domínio envolve a coleta, processamento e apresentação de dados meteorológicos.

## Requisitos Funcionais

>-Exibir Dados Meteorológicos <br>
>-Filtrar Dados Meteorológicos <br>
>-Cadastrar/Remover Sensores na Estação <br>
>-Coletar Informações dos Sensores <br>
>-Verificar Status dos Sensores <br>
>-Exportar Dados Meteorológicos <br>
>-


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
  <a href="img/UML.drawio.pdf" target="_blank">
    📄 Abrir UML
  </a>
</p>


<div align="center">

[Retroceder](README.md) | [Avançar](projeto.md)

</div>
