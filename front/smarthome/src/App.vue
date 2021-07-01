<template>
  <div id="app">
    <div id="top" class="row">cabeçalho</div>
    <!-- Cabeçalho -->

    <hr />
    <div>
      <div
        class="device row"
        v-for="dispositivo in dispositivos"
        v-bind:key="dispositivo"
      >
        <div class="col-md-4">
          <div class="alert alert-primary">Nome do dispositivo</div>
          <div class="alert alert-primary">
            {{ dispositivo.name }}
          </div>
        </div>
        <div class="col-md-4" v-if="dispositivo.status === 'on'">
          <div class="alert alert-success">Status do Dispositivo</div>
          <div class="alert alert-success">
            {{ dispositivo.status }}
          </div>
        </div>
        <div class="col-md-4" v-else>
          <div class="alert alert-danger">Status do Dispositivo</div>
          <div class="alert alert-danger">
            {{ dispositivo.status }}
          </div>
        </div>
        <div class="col-md-4" v-if="dispositivo.status === 'on'">
          <button
            type="button"
            class="btn btn-secondary"
            v-on:click="toggleLigth(dispositivo)"
          >
            Desligar
          </button>
        </div>
        <div class="col-md-4" v-else>
          <button
            type="button"
            class="btn btn-secondary"
            v-on:click="toggleLigth(dispositivo)"
          >
            Ligar
          </button>
        </div>
      </div>
      <div class="row">
      <div class="col-3">
          aaa
      </div>
      <div class="col-6">
            <form>
              <div class="form-group">
                <label for="exampleInputEmail1">Nome do dispositivo</label>
                <input
                  type="email"
                  class="form-control"
                  id="exampleInputEmail1"
                  aria-describedby="emailHelp"
                  placeholder="Nome do dispositivo"
                />
              </div>
              <div class="form-group">
                <label for="exampleInputPassword1">Nome da rede</label>
                <input
                  type="password"
                  class="form-control"
                  id="exampleInputPassword1"
                  placeholder="Nome da rede"
                />
              </div>
              <div class="form-group">
                <label for="exampleInputPassword1">Senha da rede</label>
                <input
                  type="password"
                  class="form-control"
                  id="exampleInputPassword1"
                  placeholder="Senha da rede"
                />
              </div>
              <div class="form-check">
                <input type="checkbox" class="form-check-input" id="exampleCheck1" />
                <label class="form-check-label" for="exampleCheck1"
                  >Tenho certeza que estou conectado à rede do dispositivo</label
                >
              </div>
              <button type="submit" class="btn btn-primary">Adicionar dispositivo</button>
            </form>
      </div>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: "App",
  data() {
    return {
      dispositivos: [
        {
          nome: "Luz do quarto",
          status: "ligado",
          proximoHorario: "2021/08/27",
        },
        {
          nome: "Luz da cozinha",
          status: "ligado",
          proximoHorario: "2021/08/29",
        },
        { nome: "Abajur", status: "desligado", proximoHorario: "" },
      ],
    };
  },
  methods: {
    toggleLigth(dispositivo) {
      if (dispositivo.status === "on") {
        dispositivo.status = "off";
        console.log("desligando a luz");
      } else {
        dispositivo.status = "on";
        console.log("ligando a luz");
      }
    },
    getDevices() {
      this.axios.get("device?user=flaitt1&environment=quarto").then((res) => {
        this.dispositivos = res.data;
        console.log(this.dispositivos);
      });
    },
  },
  created() {
    this.getDevices();
  },
};
</script>

<style lang="scss">
@import "./assets/styles/bootstrap";
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}
.device {
  border-style: solid;
  margin: 18px;
  padding: 64px;
}
</style>
