fetch("/output/dados.json")
  .then((res) => res.json())
  .then((data) => {
    const app = document.getElementById("app");
    const lotesContainer = document.getElementById("lotes-container");
    const lotesContainerOrdenado = document.getElementById(
      "lotes-container-ordenado",
    );

    app.innerHTML = "";
    lotesContainer.innerHTML = "";
    lotesContainerOrdenado.innerHTML = "";

    // ================= LOTES =================
    data.lotes.forEach((l) => {
      const loteDiv = document.createElement("div");
      loteDiv.className = "lote";

      loteDiv.innerHTML = `
        <img src="assets/shoes.png" class="sapatos-img">
        <div><strong>Lote ${l.id}</strong></div>
        <div>Quantidade de sapatos: ${l.qtd}</div>
        <div>Tempo por unidade: ${l.tempoUnit}</div>
        <div>Tempo total de colagem: ${l.tempoTotal}</div>
      `;

      lotesContainer.appendChild(loteDiv);
    });

    // ================= LOTES ORDENADOS =================
    const lotesOrdenados = [...data.lotes].sort(
      (a, b) => b.tempoTotal - a.tempoTotal,
    );

    lotesOrdenados.forEach((l) => {
      const loteDiv = document.createElement("div");
      loteDiv.className = "lote";

      loteDiv.innerHTML = `
        <img src="assets/shoes.png" class="sapatos-img">
        <div><strong>Lote ${l.id}</strong></div>
        <div>Quantidade de sapatos: ${l.qtd}</div>
        <div>Tempo por unidade: ${l.tempoUnit}</div>
        <div>Tempo total de colagem: ${l.tempoTotal}</div>
      `;

      lotesContainerOrdenado.appendChild(loteDiv);
    });

    // ================= MAQUINAS =================
    data.maquinas.forEach((maq, i) => {
      const maqDiv = document.createElement("div");
      maqDiv.className = "maquina";

      const info = document.createElement("div");
      info.className = "maquina-info";
      info.innerHTML = `
        <img src="assets/machine.png" class="maquina-img">
        <div>Máquina ${i + 1}</div>
        <div>Tempo de processamento: ${maq.tempoTotal}</div>
      `;

      maqDiv.appendChild(info);

      maq.lotes.forEach((l) => {
        const loteDiv = document.createElement("div");
        loteDiv.className = "lote";

        loteDiv.innerHTML = `
          <img src="assets/shoes.png" class="sapatos-img">
          <div><strong>Lote ${l.id}</strong></div>
          <div>Tempo total do lote: ${l.tempoTotal}</div>
        `;

        maqDiv.appendChild(loteDiv);
      });

      app.appendChild(maqDiv);
    });
  })
  .catch((err) => {
    console.error("Erro ao carregar JSON:", err);
  });
