<template>
  <div id="app">
    <img src="./static/home-icon.png">
    <div>Smarthome</div>
    <div id="top" class="head row">Casas > Ambientes > Dispositivos</div>
    <!-- Cabeçalho -->

    <hr />
    <div>
      <div
        class="device row"
        v-for="dispositivo in dispositivos"
        v-bind:key="dispositivo"
      >
        <div class="col-md-4">
          <div class="head-name alert alert-primary">Nome do dispositivo</div>
          <div class="alert alert-primary">
            {{ dispositivo.name }}
          </div>
        </div>
        <div class="col-md-4" v-if="dispositivo.status === 'on'">
          <div class="head-name alert alert-success">Status do Dispositivo</div>
          <div class="alert alert-success">
            {{ dispositivo.status }}
          </div>
        </div>
        <div class="col-md-4" v-else>
          <div class="head-name alert alert-danger">Status do Dispositivo</div>
          <div class="alert alert-danger">
            {{ dispositivo.status }}
          </div>
        </div>
        <div class="col-md-4" v-if="dispositivo.status === 'on'">
          <button
            type="button"
            class="button-toggle btn btn-secondary"
            v-on:click="toggleLigth(dispositivo)"
          >
            Desligar
          </button>
        </div>
        <div class="col-md-4" v-else>
          <button
            type="button"
            class="button-toggle btn btn-secondary"
            v-on:click="toggleLigth(dispositivo)"
          >
            Ligar
          </button>
        </div>
      </div>
      <div class="row">
      <div class="col-3">
      </div>
      <div class="new-device col-6">
            <form>
              <div class="form-group">
                <label for="deviceName">Nome do dispositivo</label>
                <input
                  class="form-control"
                  id="deviceName"
                  aria-describedby="emailHelp"
                  placeholder="Nome do dispositivo"
                />
              </div>
              <div class="form-group">
                <label for="ssid">Nome da rede</label>
                <input
                  class="form-control"
                  id="ssid"
                  placeholder="Nome da rede"
                />
              </div>
              <div class="form-group">
                <label for="wifiPassword">Senha da rede</label>
                <input
                  type="password"
                  class="form-control"
                  id="wifiPassword"
                  placeholder="Senha da rede"
                />
              </div>
              <div class="form-check">
                <input type="checkbox" class="form-check-input" id="exampleCheck1" />
                <label class="form-check-label" for="exampleCheck1"
                  >Tenho certeza que estou conectado à rede do dispositivo</label
                >
              </div>
              <button
                type="button"
                class="btn btn-primary"
                v-on:click="addNewDevice"
              >
              Adicionar dispositivo
              </button>
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
        // {
        //   name: "Luz do quarto",
        //   status: "ligado",
        //   proximoHorario: "2021/08/27",
        // },
        // {
        //   name: "Luz da cozinha",
        //   status: "ligado",
        //   proximoHorario: "2021/08/29",
        // },
        // { name: "Abajur", status: "desligado", proximoHorario: "" },
      ],
    };
  },
  methods: {
    toggleLigth(dispositivo) {
      const rountingKey = dispositivo.name + '_'+'quarto'+'_'+'casa'+'_'+'flaitt1';
      if (dispositivo.status === "on") {
        dispositivo.status = "off";
        console.log("deaaaasligando a luz");
        this.axios.post(`toggleStatus?action=D&routingKey=${rountingKey}`).then((res) => {
        console.log("mandou mensagem", res.data);
      }).catch((err) => {
        console.log("deu erro ao conectar", err)
      });
      } else {
        dispositivo.status = "on";
        console.log("laaaigando a luz");
        this.axios.post(`toggleStatus?action=L&routingKey=${rountingKey}`).then((res) => {
          console.log("mandou mensagem", res.data);
        }).catch((err) => {
          console.log("deu erro ao conectar", err)
        });
      }
    },
    getDevices() {
      this.axios.get("device?user=flaitt1&environment=quarto").then((res) => {
        this.dispositivos = res.data;
        console.log(res.data);
      }).catch((err) => {
        this.dispositivos = [];
        console.log("deu erro na requisição", err)
      });
    },
    addNewDevice() {
      const deviceName = document.getElementById("deviceName").value
      const ssid = document.getElementById("ssid").value
      const password = document.getElementById("wifiPassword").value
      // const endIp = Math.floor(Math.random() * 10) + 200;
      this.axiosDevice.get(`connect?rede=${ssid}&senha=${password}&routingKey=${deviceName+'_'+'quarto'+'_'+'casa'+'_'+'flaitt1'}&deviceName=${deviceName}&status=${"off"}&environment=quarto&user=flaitt1`).then((res) => {
        console.log(res.data);
        // setTimeout(() => {
        //   this.axios.post(`adddevice?name=${deviceName}&status=${"off"}&environment=quarto&user=flaitt1&endIP=${endIp}`).then((res) => {
        //      console.log(res.data);
        //   }).catch((err) => {
        //     console.log("nao conectou", err)
        //   });
        // }, 30000);
      }).catch((err) => {
        console.log("deu erro ao conectar", err)
      });
    },
    // testRabbit() {
    //   console.log("oii")
    //   try {
    //     const client = this.mqttClient.connect('ws://snake.rmq2.cloudamqp.com', {
    //           port: 1883,
    //           clientId: 'ClientId',
    //           username: 'zylppjqz:zylppjqz',
    //           password: 'bxe7Ktszi35KO9-4RHhP2pVhypiPWVk2',
    //           clean: true,
    //       });
    //       console.log("oii2")
    //       client.on('connect',function(){	
    //         console.log("connected");
    //         this.mqttClient.publish("javaTopic", "mensagem teste");
    //         console.log("connected and published");
    //       })
    //       console.log("oii3")
    //       client.on("error",function(){
    //         console.log("erro ao conectar no mqtt")
    //       })
    //   } catch (error) {
    //     console.log("error", error)
    //   }
    // }
  },
  created() {
    this.getDevices();
    // this.testRabbit();
  },
  mounted() {
    document.getElementById("wifiPassword").defaultValue = "1933425694"
    document.getElementById("ssid").defaultValue = "dd-wrt"
  }
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
.device.row{
  border-style: solid;
  margin: 42px;
  padding: 33px;
  border-radius: 35px;
  border-color: #4DD0E1;
}
.new-device {
  border-style: solid;
  padding: 35px;
  border-radius: 35px;
  border-color: #4DD0E1;
}
.head.row {
  margin-left: 30px;
}
.form-group {
  margin: 22px;
}
.head-name.alert {
  margin-bottom: 5px;
}
.button-toggle {
  margin-top: 33px;
}
</style>
