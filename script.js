let dados = {};
let geracoes = [];
let paginaAtual = 1;

let tocando = false;
let timer = null;

const INTERVALO = 500;

fetch("./output/dados.json")
  .then((res) => res.json())
  .then((json) => {
    dados = json;

    renderizarLotes();

    geracoes = Object.keys(dados).filter((chave) =>
      chave.startsWith("geracao_"),
    );

    console.log("JSON carregado:", dados);

    renderizarGeracao(1);
    renderizarPaginacao();
  })
  .catch((err) => {
    console.error("Erro ao carregar JSON:", err);
  });

function renderizarLotes() {
  const container = document.getElementById("lotes");

  if (!container || !dados.lotes) return;

  container.innerHTML = "";

  dados.lotes.forEach((lote) => {
    const card = document.createElement("div");

    card.className = "lote";

    card.innerHTML = `
      <img
        src="./assets/shoes.png"
        alt="Lote"
        class="sapatos-img"
      >

      <strong>
        Lote ${lote.id}
      </strong>

      <span>
        Quantidade:
        ${lote.qtd}
      </span>

      <span>
        Tempo Unitário:
        ${lote.tempoUnit}
      </span>

      <span>
        Tempo Total:
        ${lote.tempoTotal}
      </span>
    `;

    container.appendChild(card);
  });
}

function agruparPorMaquina(cromossomo) {
  const maquinas = {};

  cromossomo.forEach((maquina, lote) => {
    if (!maquinas[maquina]) {
      maquinas[maquina] = [];
    }

    maquinas[maquina].push(lote);
  });

  return maquinas;
}

function criarCardIndividuo(individuo, titulo) {
  const maquinas = agruparPorMaquina(individuo.cromossomo);

  const card = document.createElement("div");

  card.className = "individuo";

  let html = `
    <h3>${titulo}</h3>

    <p class="fitness">
      Fitness: ${individuo.fitness}
    </p>

    <div class="maquinas">
  `;

  Object.entries(maquinas).forEach(([idMaquina, lotes]) => {
    html += `
      <div class="maquina">

        <h4>
          Máquina ${idMaquina}
        </h4>

        <div>
          Lotes: ${lotes.join(", ")}
        </div>

      </div>
    `;
  });

  html += `
    </div>
  `;

  card.innerHTML = html;

  return card;
}

function renderizarGeracao(numero) {
  paginaAtual = numero;

  const chave = geracoes[numero - 1];

  const geracao = dados[chave];

  if (!geracao) return;

  document.getElementById("titulo-geracao").textContent = chave.replace(
    "geracao_",
    "Geração ",
  );

  const container = document.getElementById("populacao");

  container.innerHTML = "";

  geracao.populacao.forEach((individuo, indice) => {
    container.appendChild(
      criarCardIndividuo(individuo, `Indivíduo ${indice + 1}`),
    );
  });

  const melhor = document.getElementById("melhor");

  melhor.innerHTML = "";

  melhor.appendChild(criarCardIndividuo(geracao.melhor, "Melhor Indivíduo"));
}

function alternarPlay() {
  const botao = document.getElementById("btn-play");

  if (tocando) {
    clearInterval(timer);

    tocando = false;

    botao.textContent = "▶";

    return;
  }

  tocando = true;

  botao.textContent = "⏸";

  timer = setInterval(() => {
    if (paginaAtual >= geracoes.length) {
      clearInterval(timer);

      tocando = false;

      botao.textContent = "▶";

      return;
    }

    paginaAtual++;

    renderizarGeracao(paginaAtual);

    renderizarPaginacao();
  }, INTERVALO);
}

document.getElementById("btn-anterior")?.addEventListener("click", () => {
  if (paginaAtual > 1) {
    paginaAtual--;

    renderizarGeracao(paginaAtual);

    renderizarPaginacao();
  }
});

document.getElementById("btn-proximo")?.addEventListener("click", () => {
  if (paginaAtual < geracoes.length) {
    paginaAtual++;

    renderizarGeracao(paginaAtual);

    renderizarPaginacao();
  }
});

document.getElementById("btn-play")?.addEventListener("click", alternarPlay);

function renderizarPaginacao() {
  const total = geracoes.length;

  const nav = document.getElementById("paginacao");

  nav.innerHTML = "";

  const paginas = [];

  paginas.push(1);

  const inicio = Math.max(2, paginaAtual - 2);

  const fim = Math.min(total - 1, paginaAtual + 2);

  if (inicio > 2) {
    paginas.push("...");
  }

  for (let i = inicio; i <= fim; i++) {
    paginas.push(i);
  }

  if (fim < total - 1) {
    paginas.push("...");
  }

  if (total > 1) {
    paginas.push(total);
  }

  paginas.forEach((pagina) => {
    if (pagina === "...") {
      const span = document.createElement("span");

      span.className = "reticencias";

      span.textContent = "...";

      nav.appendChild(span);

      return;
    }

    const botao = document.createElement("button");

    botao.textContent = pagina;

    if (pagina === paginaAtual) {
      botao.classList.add("ativo");
    }

    botao.onclick = () => {
      renderizarGeracao(pagina);

      renderizarPaginacao();
    };

    nav.appendChild(botao);
  });
}
