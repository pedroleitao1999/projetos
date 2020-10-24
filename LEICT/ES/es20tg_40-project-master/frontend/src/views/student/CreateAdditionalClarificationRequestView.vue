<template>
    <div class="container">
        <h2>Create Additional Clarification Requests</h2>
        <v-container class="create-buttons">
            <v-container>
                <p>Create additional clarification description</p>
                <input v-model="message">
            </v-container>
            <v-container>
                <p class="pl-0">Set question id</p>
                <input v-model="message">
            </v-container>
            <v-container>
                <v-btn @click="createAdditionalClarificationRequest" depressed color="primary">
                    Create additional clarification request
                </v-btn>
            </v-container>
        </v-container>
    </div>
</template>

<script lang="ts">
import { Component, Vue } from 'vue-property-decorator';
import StatementManager from '@/models/statement/StatementManager';
import Assessment from '@/models/management/Assessment';
import RemoteServices from '@/services/RemoteServices';

@Component
export default class CreateClarificationRequestView extends Vue {

  statementManager: StatementManager = StatementManager.getInstance;
  availableAssessments: Assessment[] = [];

  async created() {
    await this.$store.dispatch('loading');
    this.statementManager.reset();
    try {
      this.availableAssessments = await RemoteServices.getAvailableAssessments();
    } catch (error) {
      await this.$store.dispatch('error', error);
    }
    await this.$store.dispatch('clearLoading');
  }

  async createAdditionalClarificationRequest() {
    try {
      await this.statementManager.getAdditionalClarificationRequestStatement();
      await this.$router.push({ name: 'solve-additionalClarificationRequest' });
    } catch (error) {
      await this.$store.dispatch('error', error);
    }
  }

};
</script>

<style scoped>

</style>