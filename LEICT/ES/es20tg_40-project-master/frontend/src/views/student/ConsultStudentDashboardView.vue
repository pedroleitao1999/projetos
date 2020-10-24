<template>
    <div class="container">
        <h2>Consult Student Dashboard</h2>
        <ul>
            <li class="list-header">
                <div class="col">Number of clarification requests</div>
                <div class="col">Number of avaliable clarification requests</div>
                <div class="col">Number of clarifications</div>
                <div class="col last-col"></div>
            </li>
            <li
                    class="list-row"
                    v-for="studentDashboard in studentDashboard"
                    :key="studentDashboard.studentDashboardId"
                    @click="showInfo(studentDashboard)"
            >
                <div class="col">
                    {{ studentDashboard.numberOfClarificationRequests }}
                </div>
                <div class="col">
                    {{ studentDashboard.numberOfAvaliableClarificationRequests }}
                </div>
                <div class="col">
                    {{ studentDashboard.numberOfClarifications }}
                </div>
                <div class="col last-col">
                    <i class="fas fa-chevron-circle-right"></i>
                </div>
            </li>
        </ul>
    </div>
</template>

<script lang="ts">
  import { Component, Vue } from 'vue-property-decorator';
  import RemoteServices from '@/services/RemoteServices';
  import StatementManager from '@/models/statement/StatementManager';
  import StatementQuiz from '@/models/statement/StatementQuiz';
  import StatementStudentDashboard from '@/models/statement/StatementStudentDashboard';

  @Component
  export default class ConsultStudentDashboardView extends Vue {
    studentDashboards: StatementStudentDashboard[] = [];

    async created() {
      await this.$store.dispatch('loading');
      try {
        this.studentDashboards = (await RemoteServices.getStudentDashboard()).reverse();
      } catch (error) {
        await this.$store.dispatch('error', error);
      }
      await this.$store.dispatch('clearLoading');
    }

    async showInfo(studentDashboard: StudentDashboard) {
      let statementManager: StatementManager = StatementManager.getInstance;
      statementManager.statementStudentDashboard = studentDashboard;
      await this.$router.push({ name: 'show-info' });
    }
  }
</script>

<style lang="scss" scoped>
    .container {
        max-width: 1000px;
        margin-left: auto;
        margin-right: auto;
        padding-left: 10px;
        padding-right: 10px;

        h2 {
            font-size: 26px;
            margin: 20px 0;
            text-align: center;
            small {
                font-size: 0.5em;
            }
        }

        ul {
            overflow: hidden;
            padding: 0 5px;

            li {
                border-radius: 3px;
                padding: 15px 10px;
                display: flex;
                justify-content: space-between;
                margin-bottom: 10px;
            }

            .list-header {
                background-color: #1976d2;
                color: white;
                font-size: 14px;
                text-transform: uppercase;
                letter-spacing: 0.03em;
                text-align: center;
            }

            .col {
                flex-basis: 25% !important;
                margin: auto; /* Important */
                text-align: center;
            }

            .list-row {
                background-color: #ffffff;
                box-shadow: 0 0 9px 0 rgba(0, 0, 0, 0.1);
                display: flex;
            }
        }
    }
</style>
