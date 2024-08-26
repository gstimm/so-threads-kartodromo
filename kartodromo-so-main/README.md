# Simulação do Kartódromo Parallel Speed

Kartódromos são instalações projetadas para a prática de kart, um tipo de corrida automobilística em miniatura. Esses locais possuem pistas asfaltadas ou de concreto, com variados formatos e comprimentos, proporcionando uma experiência desafiadora e emocionante para pilotos de todas as idades e níveis de habilidade. Além das pistas, os kartódromos oferecem áreas de apoio como boxes, manutenção, aluguel de karts e equipamentos de segurança, como capacetes.

O Kartódromo Parallel Speed solicitou seu auxílio para desenvolver um sistema que simule a distribuição dos recursos de **aluguel de karts** e **equipamentos de segurança (capacetes)** para clientes em datas especiais. Nessas ocasiões, o fluxo de pilotos aumenta, e a empresa deseja estabelecer prioridades específicas para atender os clientes.

Um exemplo de data especial é o Dia das Crianças, quando muitas crianças vão ao kartódromo com seus pais para um dia de diversão. Nesses casos, o Kartódromo Parallel Speed **precisa priorizar as crianças sem comprometer a experiência dos outros clientes.**

Seu trabalho é criar um programa concorrente (em C ou Java) que analise essa situação.

## Regras:

1. O programa deve criar grupos de threads de forma aleatória ao longo do dia, simulando grupos de pessoas que chegam no mesmo instante (informações importantes das pessoas: nome e idade).

2. Todo piloto precisa de um kart e um capacete para entrar na pista.
   2.1 Considerando que as crianças podem ter mais dificuldade em encontrar um capacete adequado e adultos preferem escolher a motorização do kart. **Adote como regra que toda criança buscará primeiro o capacete e o adulto buscará primeiro o kart.**

3. Cada piloto, representado por thread, de posse dos dois recursos, poderá entrar na pista por um período de **tempo determinado e aleatório (tempos distintos por piloto).**

4. Considere que o kartódromo possui 10 karts disponíveis e apenas 10 capacetes. Na fila para pegar capacetes, **crianças até 14 anos possuem prioridade em relação as outras crianças.**

5. O sistema implementado deve simular a ideia de dia (por exemplo, 8 horas de funcionamento), imprimindo o que acontece ao longo do dia e no final do dia deverá imprimir um relatório contendo: o total de clientes atendidos, tempo médio de espera, clientes que ficaram na fila e não foram atendidos (apresente o tempo de espera), quantidade de vezes que cada recurso foi utilizado (capacete e kart), além de algum outro dado que considere relevante.
