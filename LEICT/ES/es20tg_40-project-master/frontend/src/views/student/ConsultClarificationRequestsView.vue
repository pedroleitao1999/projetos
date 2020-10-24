<template>
    <div class="container">
        <h2>Consult Clarification Requests</h2>
        <v-container class="create-buttons">
            <v-container>
                <p class="pl-0">Set question id</p>
                <input v-model="message">
            </v-container>
            <h2>Clarification Requests</h2>
            <ul>
                <li class="list-header">
                    <div class="col">Id</div>
                    <div class="col">Description</div>
                    <div class="col last-col"></div>
                </li>
                <li
                        class="list-row"
                        v-for="clarificationRequest in clarificationRequests"
                        :key="clarificationRequest.getElementById()"
                        @click="findClarificationRequests(clarificationRequest)"
                >
                    <div class="col">
                        {{ clarificationRequest.id }}
                    </div>
                    <div class="col">
                        {{ clarificationRequest.description }}
                    </div>
                    <div class="col last-col">
                        <i class="fas fa-chevron-circle-right"></i>
                    </div>
                </li>
            </ul>
        </v-container>
    </div>
</template>

<script lang="ts">
import { Component, Vue } from 'vue-property-decorator';
import RemoteServices from '@/services/RemoteServices';
import StatementManager from '@/models/statement/StatementManager';
import StatementQuiz from '@/models/statement/StatementQuiz';
import StatementClarificationRequest from '@/models/statement/StatementClarificationRequest';
import StatementQuestion from '@/models/statement/StatementQuestion';

  @Component
  export default class ConsultClarificationRequestsView extends Vue {
    clarificationRequests: StatementClarificationRequests[] = [];

    async created() {
      await this.$store.dispatch('loading');
      try {
        this.quizzes = (await RemoteServices.getAvailableQuizzes()).reverse();
      } catch (error) {
        await this.$store.dispatch('error', error);
      }
      await this.$store.dispatch('clearLoading');
    }

    async findClarificationRequests(clarificationRequest: StatementClarificationRequest) {
      let statementManager: StatementManager = StatementManager.getInstance;
      statementManager.statementClarificationRequest = clarificationRequest;
      await this.$router.push({ name: 'find-clarification-requests' });
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
