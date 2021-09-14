<template>
  <div id="houses">
    <img src="./../static/home-icon.png">
    <div>Smartcommand</div>
    <div id="top" class="head row">Casas > Ambientes > Dispositivos</div>
    <!-- Cabeçalho -->
    <hr />
    <div>
      <div class="spinner-border text-info" v-if="showHouseLoading"></div>
      <div
        class="device row"
        v-for="house in houses"
        v-bind:key="house"
        v-else
      >
        <div class="col-md-4">
          <div class="head-name alert alert-primary">Nome do dispositivo</div>
          <div class="alert alert-primary">
            {{ dispositivo.name }}
          </div>
        </div>
      </div>
      <div class="row">
      <div class="col-3">
      </div>
      <button
        type="button"
        class="new-device col-6"
        v-if="!showAddNewDevice"
        v-on:click="openAddNewDevice()"
      >
      Adicionar novo dispositivo
      </button>
      <div class="new-device col-6" v-else>
        <div class="spinner-border text-info" v-if="showNewDeviceLoading"></div>
        <div class="addNewDevice" v-else>
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
              <button
                type="button"
                class="close btn btn-secondary"
                v-on:click="closeAddNewDevice"
              >
              Fechar
              </button>
            </form>
        </div>
      </div>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: "App",
  props: {
    'userName': {
      type: String,
      required: true
    }
  },
  data() {
    return {
      dispositivos: [],
      showAddNewDevice: false,
      showHouseLoading: true,
      showNewDeviceLoading: false,
      checkDeviceStatus: true
    };
  },
  methods: {
    toggleLigth(dispositivo) {
      const rountingKey = dispositivo.name + '_'+'quarto'+'_'+'casa'+'_'+this.userName;
      this.checkDeviceStatus = false;
      setTimeout(() => {this.checkDeviceStatus=true},4000 )
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
        this.axios.post(`toggleStatus?action=L&routingKey=${rountingKey}`).then((res) => {
          console.log("mandou mensagem", res.data);
        }).catch((err) => {
          console.log("deu erro ao conectar", err)
        });
      }
    },
    getDevices() {
      if(this.checkDeviceStatus){
        setTimeout(() => {
          this.axios.get(`device?user=${this.userName}&environment=quarto`).then((res) => {
            this.dispositivos = res.data;
            this.showDeviceLoading = false;
            console.log("dispositivos obtidos");
          }).catch((err) => {
            this.dispositivos = [];
            console.log("deu erro na requisição", err)
          });
          this.getDevices();
        }, 4000);
      } else {
        setTimeout(()=>{this.getDevices()}, 1000)
      }
    },
    addNewDevice() {
      const deviceName = document.getElementById("deviceName").value
      const ssid = document.getElementById("ssid").value
      const password = document.getElementById("wifiPassword").value
      this.showNewDeviceLoading = true;
      this.checkDeviceStatus = false;
      this.axiosDevice.get(`connect?rede=${ssid}&senha=${password}&routingKey=${deviceName+'_'+'quarto'+'_'+'casa'+'_'+this.userName}&deviceName=${deviceName}&status=${"off"}&environment=quarto&user=${this.userName}`).then((res) => {
        console.log(res.data);
      }).catch((err) => {
        console.log("deu erro ao conectar", err)
      });
      setTimeout(() => {
          this.axios.get(`device?user=${this.userName}&environment=quarto`).then((res) => {
            this.dispositivos = res.data;
            this.showNewDeviceLoading = false;
            this.showAddNewDevice =false;
            if(this.dispositivos.filter(dispositivo => dispositivo.name === deviceName).length > 0) {
              alert("Dispositivo adicionado!")
            } else {
              alert("Erro ao adicionar dispositivo, tente novamente!")
            }
          }).catch((err) => {
            this.dispositivos = [];
            console.log("deu erro na requisição", err)
        });
        this.checkDeviceStatus = true;
        }, 10000);
    },
    openAddNewDevice(){
      this.showAddNewDevice = true;
    },
    closeAddNewDevice(){
      this.showAddNewDevice = false;
    }
  },
  created() {
    this.getDevices();
    console.log('props.userName:  ', this.userName);
    if(this.userName === undefined) {
      console.log("entrou undefined username")
      this.$router.replace({name: 'login'});
    }
  }
};
</script>

<style lang="scss">
@import "./../assets/styles/bootstrap";
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
.spinner-border.text-info {
  margin: 28px;
  padding: 17px;
}
.close.btn.btn-secondary {
  margin-left: 193px;
}
.form-check-label {
  margin-right: 150px;
}
</style>
